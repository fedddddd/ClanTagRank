#include "pch.h"
#include <Windows.h>
#include <iostream>
#include <string>
#include "functions.h"
#include <stdio.h>
#include <cstdlib>
#include <chrono>
#include <thread>



void init()
{
    
    std::cout << "ClanTagRank (1.1) by INSANEMODE\n";
    std::cout << "loaded iw4m-admin url from .\\Plugins\\GetClanTag.ini: " << Game::configfile() << '\n';
    std::thread rank(Game::clanTagThread);
    rank.detach();
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        LogHandle = GetModuleHandle(NULL);
        init();
    }
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
