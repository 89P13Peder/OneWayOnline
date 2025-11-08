// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ZombieAI.generated.h"

UCLASS()
class ONEWAYGAME_API AZombieAI : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AZombieAI();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSeePawn(APawn* Pawn);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	class UPawnSensingComponent* PawnSensingComp;

};
