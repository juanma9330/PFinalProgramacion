// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CharacterHud.h"

#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Framework/PF_GameState.h"

void UCharacterHud::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (Image_PickupItem) Image_PickupItem->SetVisibility(ESlateVisibility::Hidden);
	if (Text_PickupName) Text_PickupName->SetVisibility(ESlateVisibility::Hidden);
	if (GanadorText) GanadorText->SetVisibility(ESlateVisibility::Hidden);
	
	if (APF_GameState* GS = GetWorld()->GetGameState<APF_GameState>())
	{
		GS->OnPuntajeEquipoChanged.AddDynamic(this, &UCharacterHud::OnPuntajeEquipoChanged);
		GS->OnGanadorName.AddDynamic(this, &UCharacterHud::SetEquipoGanador);
				
		SetScoreAzulText(GS->GetPuntajeEquipo(ETeam::Blue));
		SetScoreRojoText(GS->GetPuntajeEquipo(ETeam::Red));
		SetTiempo(GS->GetTiempoRestante());
		
		GS->OnTiempoRestanteChanged.AddDynamic(this, &UCharacterHud::SetTiempo);

	}
}

void UCharacterHud::SetScoreAzulText(int32 ScoreAzul)
{
	if (Text_ScoreAzul)
	{
		Text_ScoreAzul->SetText(FText::FromString(FString::Printf(TEXT("Azul: %d"), ScoreAzul)));
		
	}
}

void UCharacterHud::SetScoreRojoText(int32 ScoreRojo)
{
	if (Text_ScoreRojo)
	{
		Text_ScoreRojo->SetText(FText::FromString(FString::Printf(TEXT("Rojo: %d"), ScoreRojo)));
	}
}

void UCharacterHud::SetTiempo(int32 TiempoPartida)
{
	if (TiempoText)
	{
		int32 Minutos = TiempoPartida / 60;
		int32 Segundos = TiempoPartida % 60;
		TiempoText->SetText(FText::FromString(FString::Printf(TEXT("%02d:%02d"), Minutos, Segundos)));
	}
}

void UCharacterHud::SetEquipoGanador(ETeam EquipoGanador)
{
	if (!GanadorText) return;
	
	FString TextoGanador;
	FSlateColor ColorTexto;

	if (EquipoGanador == ETeam::Blue)
	{
		TextoGanador = "VICTORIA DEL EQUIPO AZUL!";
		ColorTexto = FSlateColor(FLinearColor::Blue);
	}
	else if (EquipoGanador == ETeam::Red)
	{
		TextoGanador = "VICTORIA DEL EQUIPO ROJO!";
		ColorTexto = FSlateColor(FLinearColor::Red);
	}
	else
	{
		TextoGanador = "EMPATE!";
		ColorTexto = FSlateColor(FLinearColor::White);
	}

	GanadorText->SetText(FText::FromString(TextoGanador));
	GanadorText->SetColorAndOpacity(ColorTexto);
	GanadorText->SetVisibility(ESlateVisibility::Visible);
}

void UCharacterHud::MostrarPromptItem(bool bMostrar, FString NombreItem)
{
}

void UCharacterHud::OnPuntajeEquipoChanged(ETeam Equipo, int32 NuevoPuntaje)
{
	if (Equipo == ETeam::Blue)
	{
		SetScoreAzulText(NuevoPuntaje);
	}
	else if (Equipo == ETeam::Red)
	{
		SetScoreRojoText(NuevoPuntaje);
	}
	if (APF_GameState* GS = GetWorld()->GetGameState<APF_GameState>())
	{
		ActualizarBarraDominio(GS->GetPuntajeEquipo(ETeam::Blue), GS->GetPuntajeEquipo(ETeam::Red));
	}
}

void UCharacterHud::ActualizarBarraDominio(int32 PuntajeAzul, int32 PuntajeRojo)
{
	if (!ProgressBar_Dominio) return;

	const int32 Total = PuntajeAzul + PuntajeRojo;

	if (Total <= 0)
	{
		ProgressBar_Dominio->SetPercent(0.5f);
		return;
	}

	// Porcentaje azul sobre el total → fill azul avanza sobre fondo rojo
	ProgressBar_Dominio->SetPercent((float)PuntajeAzul / (float)Total);
}
