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

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	


	
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
	
	// ray-casting out to reach distance (private var)
	DrawDebugLine(GetWorld(), PlayerViewPointLocation, LineTraceEnd, FColor(255, 0, 0), false, 0.f, 0.f, 10.f);

	// setup query parameters 
	FCollisionQueryParams TraceParameters (FName(TEXT("")), false, GetOwner());

	//line-trace (aka ray-cast) out to reach distance

	FHitResult Hit;
	

	GetWorld()->LineTraceSingleByObjectType(OUT Hit, PlayerViewPointLocation, LineTraceEnd, FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParameters);

	// see what we hit 

	AActor* ActorHit = Hit.GetActor();

	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line Trace Hit: %s"), *(ActorHit->GetName()));
	}
}

