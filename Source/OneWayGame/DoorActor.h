#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "DoorActor.generated.h"

UCLASS()
class ONEWAYGAME_API ADoorActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ADoorActor();

protected:
	virtual void BeginPlay() override;

	// Componentes (NO replicar explícitamente)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* DoorMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* InteractionComponent;

	// Propiedad replicada para saber si la puerta está abierta
	UPROPERTY(ReplicatedUsing = OnRep_IsOpen, BlueprintReadOnly, Category = "Door")
	bool bIsOpen;

	UFUNCTION()
	void OnRep_IsOpen();

	// Función para manejar overlap
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	virtual void Tick(float DeltaTime) override;

	// Función para interactuar con la puerta
	UFUNCTION(BlueprintCallable, Category = "Door")
	void InteractWithDoor(class AOneWayGameCharacter* InteractingPlayer);

	// Getter para el estado de la puerta
	UFUNCTION(BlueprintPure, Category = "Door")
	bool GetIsOpen() const { return bIsOpen; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};