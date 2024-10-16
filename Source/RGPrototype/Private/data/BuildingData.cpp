// Fill out your copyright notice in the Description page of Project Settings.


#include "data/BuildingData.h"

FBuilding& UBuildingData::GetBuildingByTag(FGameplayTag Tag)
{
	for (FBuilding& building : Buildings)
	{
		if (building.BuildingTag.MatchesTagExact(Tag))
		{
			return building;
		}
	}
	return Buildings[0];
}
