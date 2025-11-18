#include "EnemySpawner.h"

AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AActor* AEnemySpawner::SpawnEnemy()
{
	if (!HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnEnemy llamado sin autoridad"));
		return nullptr;
	}

	if (!EnemyToSpawn)
	{
		UE_LOG(LogTemp, Error, TEXT("EnemySpawner: No hay clase asignada en EnemyToSpawn"));
		return nullptr;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	FVector SpawnLocation = GetActorLocation();
	FRotator SpawnRotation = GetActorRotation();

	AActor* SpawnedEnemy = GetWorld()->SpawnActor<AActor>(EnemyToSpawn, SpawnLocation, SpawnRotation, SpawnParams);

	if (SpawnedEnemy)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemySpawner: Enemigo spawneado -> %s"), *SpawnedEnemy->GetName());
	}

	return SpawnedEnemy;
}

