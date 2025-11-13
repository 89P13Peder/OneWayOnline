#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"
#include "WinZoneActor.generated.h"

UCLASS()
class ONEWAYGAME_API AWinZoneActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AWinZoneActor();

protected:
	virtual void BeginPlay() override;

	// Componente de colisión para detectar jugadores
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* WinZoneCollision;

	// Propiedad para llevar cuenta de qué jugadores ya ganaron
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Win Zone")
	TArray<class AOneWayGameCharacter*> PlayersWhoWon;

	// Función para manejar overlap
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	virtual void Tick(float DeltaTime) override;

	// Replicación
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
