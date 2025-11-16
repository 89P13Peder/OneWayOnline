// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "OneWayGameCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  A simple player-controllable third person character
 *  Implements a controllable orbiting camera
 */
UCLASS(abstract)
class AOneWayGameCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
protected:

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MouseLookAction;

public:

	/** Constructor */
	AOneWayGameCharacter();	

protected:

	/** Initialize input action bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

public:

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles look inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();

public:

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:
	// Propiedad replicada para saber si el jugador tiene la llave
	UPROPERTY(ReplicatedUsing = OnRep_HasKey, BlueprintReadOnly, Category = "Player")
	bool bHasKey;

	UPROPERTY(ReplicatedUsing = OnRep_HasKey, BlueprintReadOnly, Category = "Player")
	bool bHasWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* WeaponMesh;

	// Función de replicación
	UFUNCTION()
	void OnRep_HasKey();
	
	UFUNCTION()
	void OnRep_HasWeapon();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerDestroyActor(AActor* TargetActor);

	UFUNCTION(BlueprintCallable, BlueprintCallable, Category="Interaction")
	void DestroyItem(AActor* TargetActor);

	UFUNCTION(Server, Reliable)
	void ServerSetNewStaticMesh(UStaticMesh* NewMesh);

	UFUNCTION(BlueprintCallable, Category="Interaction")
	void SetNewStaticMesh(UStaticMesh* NewMesh);

public:

	/** Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* InteractAction;
	
	// Setter para la llave (solo en servidor)
	UFUNCTION(BlueprintCallable, Category = "Player")
	void SetHasKey(bool bNewHasKey);

	// Getter para la llave
	UFUNCTION(BlueprintPure, Category = "Player")
	bool GetHasKey() const { return bHasKey; }
	
	// Setter para la llave (solo en servidor)
	UFUNCTION(BlueprintCallable, Category = "Player")
	void SetHasWeapon(bool bNewHasKey);

	// Getter para la llave
	UFUNCTION(BlueprintPure, Category = "Player")
	bool GetHasWeapon() const { return bHasKey; }

	// Replicación
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	// Funciones para mostrar widgets (ejecutadas en todos los clientes)
	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "UI")
	void ShowWinWidget();
	
	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "UI")
	void ShowLoseWidget();
	
	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "UI")
	void ShowNeedKeyWidget();
	
	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "UI")
	void HideAllWidgets();

	UFUNCTION()
	void DoInteract();
	
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void CheckOverlappingItems();

	// Widget classes para spawnear
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UUserWidget> WinWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UUserWidget> LoseWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UUserWidget> NeedKeyWidgetClass;

private:
	// Referencias a los widgets actuales
	UPROPERTY()
	UUserWidget* CurrentWidget;

private:
	UFUNCTION(BlueprintCallable, Category = "Player")
	APlayerController* GetPlayerController() const;
};

