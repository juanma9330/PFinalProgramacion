// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Framework/PF_PlayerState.h"

#include "Net/UnrealNetwork.h"

void APF_PlayerState::SetTeamColor(ETeam Team)
{
	TeamColor = Team;
}

ETeam APF_PlayerState::GetTeamColor() const
{
	return TeamColor;
}

void APF_PlayerState::OnRep_TeamColor()
{
}

void APF_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, TeamColor);
	DOREPLIFETIME(ThisClass, ValorTotalLoot);
	DOREPLIFETIME(ThisClass, ItemsReclectados);
}

void APF_PlayerState::AddLoot(int32 SumaLoot)
{
	if (!HasAuthority()) return;
	
	ValorTotalLoot += SumaLoot;
	ItemsReclectados++;
	OnLootValueChanged.Broadcast(ValorTotalLoot);
	OnItemsCollectedChanged.Broadcast(ItemsReclectados);

}

void APF_PlayerState::OnRep_ValorTotalLoot()
{
	OnLootValueChanged.Broadcast(ValorTotalLoot);
}
 
void APF_PlayerState::OnRep_ItemsReclectados()
{
	OnItemsCollectedChanged.Broadcast(ItemsReclectados);
}