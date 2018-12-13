// Fill out your copyright notice in the Description page of Project Settings.

#include "Mannequin.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ChildActorComponent.h"
#include "Components/InputComponent.h"
#include "../Weapons/Rifle.h"

// Sets default values
AMannequin::AMannequin()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a CameraComponent
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(5.0f, 3.5f, 64.0f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);
}

// Called when the game starts or when spawned
void AMannequin::BeginPlay()
{
	Super::BeginPlay();

	if(RifleBlueprint != nullptr)
	{
		Rifle = GetWorld()->SpawnActor<ARifle>(RifleBlueprint);

		//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
		if(IsPlayerControlled())
		{
			Rifle->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
		}
		else 
		{ 
			Rifle->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint")); 
		}
		
		Rifle->AnimInstance1P = Mesh1P->GetAnimInstance();
		Rifle->AnimInstance3P = GetMesh()->GetAnimInstance();
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Rifle Blueprint Missing.")); }
	
}

// Called every frame
void AMannequin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMannequin::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InputComponent->BindAction("Fire", IE_Pressed, this, &AMannequin::PullTrigger);
}

void AMannequin::UnPossessed() 
{
	Super::UnPossessed();

	if(Rifle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Gun is not available"))
			return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Gun re-attached"))
	Rifle->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
}

void AMannequin::PullTrigger()
{
	if(Rifle != nullptr)
	{
		Rifle->OnFire();
	}
}