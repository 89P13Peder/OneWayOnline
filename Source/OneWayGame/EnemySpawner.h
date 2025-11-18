
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

UCLASS()
class ONEWAYGAME_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemySpawner();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	/** Clase del enemigo a spawnear (editable desde el Blueprint) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
	TSubclassOf<AActor> EnemyToSpawn;

	/** Función que hace spawn del enemigo */
	UFUNCTION(BlueprintCallable, Category = "Spawner")
	AActor* SpawnEnemy();

};
