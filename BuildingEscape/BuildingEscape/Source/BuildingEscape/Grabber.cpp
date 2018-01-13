// Copyright Limping Flower Game 2018

#include "Grabber.h"
#include "Gameframework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/Controller.h"
#include "DrawDebugHelpers.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
}

void UGrabber::FindPhysicsHandleComponent()
{
	// looking for attached physics handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		//physics handle is found
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing Physics Handle Component"), *GetOwner()->GetName());

	}
}

void UGrabber::SetupInputComponent()
{
	// looking for attached input component
	InputComponnent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponnent)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s has Input Component attached"), *GetOwner()->GetName());
		InputComponnent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponnent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		// nothing for now
	}
}

void UGrabber::Grab()
{
	// called every frame
	UE_LOG(LogTemp, Warning, TEXT("Grab press"));

	//line trace and reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	//if we hit something then attach a physics handle

	if (ActorHit)
	{
		PhysicsHandle->GrabComponentAtLocation(ComponentToGrab, NAME_None, GetOwner()->GetActorLocation());
	
	}
}

void UGrabber::Release()

{
	UE_LOG(LogTemp, Warning, TEXT("Grab is released"));

	PhysicsHandle->ReleaseComponent();
	// TODO release physics handle

}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// get the player viewpoint this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint
	(
		OUT PlayerViewPointLocation, OUT PlayerViewPointRotation
	);

	FString ViewLocation = PlayerViewPointLocation.ToString();
	FString ViewRotation = PlayerViewPointRotation.ToString();

	//UE_LOG(LogTemp, Warning, TEXT("Location: %s, Rotation: %s"), *ViewLocation, *ViewRotation);

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	// if the physics handle is attached

	if (PhysicsHandle->GrabbedComponent)
	{
		//move the object that we're hoding
		PhysicsHandle->SetTargetLocation(LineTraceEnd);

	}
	
	
}
const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	// get the player viewpoint this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint
	(
		OUT PlayerViewPointLocation, OUT PlayerViewPointRotation
	);

	FString ViewLocation = PlayerViewPointLocation.ToString();
	FString ViewRotation = PlayerViewPointRotation.ToString();

	//UE_LOG(LogTemp, Warning, TEXT("Location: %s, Rotation: %s"), *ViewLocation, *ViewRotation);

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	// ray-casting out to reach distance (private var)
	//DrawDebugLine(GetWorld(), PlayerViewPointLocation, LineTraceEnd, FColor(255, 0, 0), false, 0.f, 0.f, 10.f);

	// setup query parameters 
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	//line-trace (aka ray-cast) out to reach distance

	FHitResult Hit;

	GetWorld()->LineTraceSingleByObjectType(OUT Hit, PlayerViewPointLocation, LineTraceEnd, FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParameters);

	// see what we hit 

	AActor* ActorHit = Hit.GetActor();

	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line Trace Hit: %s"), *(ActorHit->GetName()));
	}

	return Hit;
}
