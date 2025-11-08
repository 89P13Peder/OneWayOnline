// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieAI.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController_Enemy.h"
#include "OneWayGameCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Slate/SGameLayerManager.h"

// Sets default values
AZombieAI::AZombieAI()
{
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

	// Intentar castear al personaje del jugador
	AOneWayGameCharacter* PlayerCharacter = Cast<AOneWayGameCharacter>(Pawn);
	if (!PlayerCharacter) return;
	if (PlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Zombie vio al jugador: %s"), *PlayerCharacter->GetName());
	}
	
	AAIController* AIController = Cast<AAIController>(GetController());
	if (!AIController) return;

	UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
	if (!BlackboardComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("No se encontró el BlackboardComponent"));
		return;
	}

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

