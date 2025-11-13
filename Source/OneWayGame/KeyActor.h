// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"
#include "KeyActor.generated.h"

UCLASS()
class ONEWAYGAME_API AKeyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKeyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Componente de malla
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Components")
	class UStaticMeshComponent* KeyMesh;

	// Componente de colisión
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Components")
	class USphereComponent* CollisionComponent;

	// Propiedad replicada para saber si la llave fue recolectada
	UPROPERTY(ReplicatedUsing = OnRep_IsCollected, BlueprintReadOnly, Category = "Key")
	bool bIsCollected;

	// Función de replicación
	UFUNCTION()
	void OnRep_IsCollected();

	// Función para manejar overlap
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Función para recoger la llave
	UFUNCTION(BlueprintCallable, Category = "Key")
	void CollectKey(class AOneWayGameCharacter* CollectingPlayer);

	// Getter para saber si la llave fue recolectada
	UFUNCTION(BlueprintPure, Category = "Key")
	bool GetIsCollected() const { return bIsCollected; }

	// Replicación
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};