// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieAI.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController_Enemy.h"
#include "OneWayGameCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Slate/SGameLayerManager.h"

// Sets default values
AZombieAI::AZombieAI()
{
	bReplicates = true;
	ACharacter::SetReplicateMovement(true);
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//AI Controller:
	AIControllerClass  = AAIController_Enemy::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	//Pawn sensing:
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
	PawnSensingComp->SightRadius = 1000.f;         
	PawnSensingComp->SetPeripheralVisionAngle(60.f);
	PawnSensingComp->HearingThreshold = 600.f;

	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Overlap);

	GetMesh()->OnComponentBeginOverlap.AddDynamic(this, &AZombieAI::OnWeaponOverlap);
}

// Called when the game starts or when spawned
void AZombieAI::BeginPlay()
{
	Super::BeginPlay();

	if (PawnSensingComp)
	{
		PawnSensingComp->OnSeePawn.AddDynamic(this, &AZombieAI::OnSeePawn);
	}
	
}

void AZombieAI::OnSeePawn(APawn* Pawn)
{
	if (!Pawn) return;

	AOneWayGameCharacter* PlayerCharacter = Cast<AOneWayGameCharacter>(Pawn);
	if (!PlayerCharacter) return;

	UE_LOG(LogTemp, Warning, TEXT("Zombie vio al jugador: %s"), *PlayerCharacter->GetName());

	AAIController* AIController = Cast<AAIController>(GetController());
	if (!AIController) return;

	UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
	if (!BlackboardComp) return;

	BlackboardComp->SetValueAsObject(TEXT("PlayerActor"), PlayerCharacter);
	BlackboardComp->SetValueAsBool(TEXT("bHasSeenPlayer"), true);
}

// Called every frame
void AZombieAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AZombieAI::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AZombieAI::OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority()) return;

	if (!OtherComp) return;

	if (!OtherComp->ComponentHasTag("Sword"))
		return;

	AOneWayGameCharacter* Attacker = Cast<AOneWayGameCharacter>(OtherComp->GetOwner());
	if (!Attacker) return;

	UE_LOG(LogTemp, Warning, TEXT("Zombie recibió golpe de espada: %s"),
		   *Attacker->GetName());

	UGameplayStatics::ApplyDamage(
		this,            
		20.0f,           
		Attacker->GetController(),
		Attacker,
		UDamageType::StaticClass()
	);
}

