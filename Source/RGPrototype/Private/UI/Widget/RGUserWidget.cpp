// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/RGUserWidget.h"

void URGUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
