// Fill out your copyright notice in the Description page of Project Settings.

#include "FlytekVoiceSDK.h"
#include "SpeekRecognizerFunctionLibrary.h"


USpeechRecognizer* USpeekRecognizerFunctionLibrary::InitializeSpeechRecognizer(ESpeechLanguage InLanguage, bool IsContinuous)
{
	return IFlytekVoiceSDK::Get().InitializeSpeechRecognize(InLanguage,IsContinuous);
}
void USpeekRecognizerFunctionLibrary::StartLisening()
{
	IFlytekVoiceSDK::Get().SpeechRecStartListening();
}
void USpeekRecognizerFunctionLibrary::StopLisening()
{
	IFlytekVoiceSDK::Get().SpeechRecStopListening();
}
