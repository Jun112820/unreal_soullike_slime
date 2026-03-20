#pragma once

namespace SLUtil
{
	static bool MeleeTraceMulti(UWorld* World, AActor* IgnoredActor, const FVector& Start, const FVector& End, float Radius, TArray<FHitResult>& OutHits, bool bDrawDebug = false)
	{
		if (!World) return false;

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(IgnoredActor);

		// 1. Sweep 실행
		bool bHit = World->SweepMultiByChannel(
			OutHits, Start, End, FQuat::Identity, 
			ECC_Pawn, FCollisionShape::MakeSphere(Radius), Params
		);

		// 2. 디버그 드로잉
		if (bDrawDebug)
		{
			FColor DebugColor = bHit ? FColor::Red : FColor::Green;
			FVector Center = (Start + End) * 0.5f;
			float HalfHeight = (FVector::Dist(Start, End) * 0.5f) + Radius;
			FQuat CapsuleRot = FRotationMatrix::MakeFromZ(End - Start).ToQuat();

			DrawDebugCapsule(World, Center, HalfHeight, Radius, CapsuleRot, DebugColor, false, 2.0f);
		}

		return bHit;
	}
}