// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

void OnProcessAttach();
void OnThreadAttach();
void OnProcessDetach();
void OnThreadDetach();

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		OnProcessAttach() ;
		break ;
	case DLL_THREAD_ATTACH:
		OnThreadAttach() ;
		break ;
	case DLL_THREAD_DETACH:
		OnThreadDetach() ;
		break ;
	case DLL_PROCESS_DETACH:
		OnProcessDetach() ;
		break;
	}
	return TRUE;
}

void OnProcessAttach()
{
	
}

void OnThreadAttach()
{

}

void OnProcessDetach()
{

}

void OnThreadDetach()
{

}