// Copyright Epic Games, Inc. All Rights Reserved.

#include "OneWayGameCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "OneWayGame.h"
#include "Net/UnrealNetwork.h"
#include "Blueprint/UserWidget.h"
#include "TimerManager.h"
#include "Engine/World.h" 

AOneWayGameCharacter::AOneWayGameCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(GetMesh(), TEXT("WeaponSocket"));
	WeaponMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}

void AOneWayGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AOneWayGameCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AOneWayGameCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AOneWayGameCharacter::Look);
	}
	else
	{
		UE_LOG(LogOneWayGame, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AOneWayGameCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void AOneWayGameCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void AOneWayGameCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void AOneWayGameCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AOneWayGameCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void AOneWayGameCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}

void AOneWayGameCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    DOREPLIFETIME(AOneWayGameCharacter, bHasKey);
}

void AOneWayGameCharacter::OnRep_HasKey()
{
    UE_LOG(LogTemp, Warning, TEXT("Player %s key status: %s"), *GetName(), bHasKey ? TEXT("Has Key") : TEXT("No Key"));
}

void AOneWayGameCharacter::SetHasKey(bool bNewHasKey)
{
    if (HasAuthority())
    {
        bHasKey = bNewHasKey;
        OnRep_HasKey();
    }
}

void AOneWayGameCharacter::ShowWinWidget_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("ShowWinWidget_Implementation ejecutado"));
    
	if (!WinWidgetClass || !IsLocallyControlled()) 
		return;
    
	APlayerController* PC = GetPlayerController();
	if (!PC) 
		return;
    
	// Versión mínima
	HideAllWidgets();
    
	CurrentWidget = CreateWidget<UUserWidget>(PC, WinWidgetClass);
	if (CurrentWidget)
	{
		CurrentWidget->AddToViewport(999); // Z-Order muy alto
        
		// Solo el cursor por ahora
		PC->SetShowMouseCursor(true);
		PC->SetInputMode(FInputModeUIOnly());
        
		UE_LOG(LogTemp, Warning, TEXT(" Widget mostrado (versión mínima)"));
	}
}

void AOneWayGameCharacter::ShowLoseWidget_Implementation()
{
    UE_LOG(LogTemp, Warning, TEXT("ShowLoseWidget called for %s"), *GetName());
    
    if (LoseWidgetClass && IsLocallyControlled())
    {
        HideAllWidgets();
        UWorld* World = GetWorld();
        if (World)
        {
            CurrentWidget = CreateWidget<UUserWidget>(GetPlayerController(), LoseWidgetClass);
            if (CurrentWidget)
            {
                CurrentWidget->AddToViewport();
                
                if (APlayerController* PC = GetPlayerController())
                {
                    PC->SetShowMouseCursor(true);
                    FInputModeUIOnly InputMode;
                    InputMode.SetWidgetToFocus(CurrentWidget->TakeWidget());
                    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
                    PC->SetInputMode(InputMode);
                }
            }
        }
    }
}

void AOneWayGameCharacter::ShowNeedKeyWidget_Implementation()
{
    UE_LOG(LogTemp, Warning, TEXT("ShowNeedKeyWidget called for %s"), *GetName());
    
    if (NeedKeyWidgetClass && IsLocallyControlled())
    {
        HideAllWidgets();
        UWorld* World = GetWorld();
        if (World)
        {
            CurrentWidget = CreateWidget<UUserWidget>(GetPlayerController(), NeedKeyWidgetClass);
            if (CurrentWidget)
            {
                CurrentWidget->AddToViewport();
                
                FTimerHandle TimerHandle;
                World->GetTimerManager().SetTimer(TimerHandle, this, &AOneWayGameCharacter::HideAllWidgets, 2.0f, false);
            }
        }
    }
}

void AOneWayGameCharacter::HideAllWidgets_Implementation()
{
    if (CurrentWidget)
    {
        CurrentWidget->RemoveFromParent();
        CurrentWidget = nullptr;
        
        if (APlayerController* PC = GetPlayerController())
        {
            PC->SetShowMouseCursor(false);
            FInputModeGameOnly InputMode;
            PC->SetInputMode(InputMode);
        }
    }
}

APlayerController* AOneWayGameCharacter::GetPlayerController() const
{
    return Cast<APlayerController>(GetController());
}