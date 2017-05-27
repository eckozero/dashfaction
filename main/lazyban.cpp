#include "stdafx.h"
#include "lazyban.h"
#include "utils.h"
#include "rf.h"
#include "commands.h"

using namespace rf;

void BanCmdHandlerHook()
{
    if (*g_pbNetworkGame && *g_pbLocalNetworkGame)
    {
        if (*g_pbDcRun)
        {
            CPlayer *pPlayer;
            
            rf::DcGetArg(DC_ARG_STR, 1);
            pPlayer = FindBestMatchingPlayer(g_pszDcArg);
            if (pPlayer)
            {
                if (pPlayer != *g_ppLocalPlayer)
                {
                    DcPrintf(g_ppszStringsTable[959], pPlayer->strName.psz);
                    BanIp(&(pPlayer->pNwData->Addr));
                    KickPlayer(pPlayer);
                } else
                    DcPrintf("You cannot ban yourself!");
            }
        }
        
        if (*g_pbDcHelp)
        {
            DcPrint(g_ppszStringsTable[STR_USAGE], NULL);
            DcPrintf("     ban <%s>", g_ppszStringsTable[STR_PLAYER_NAME]);
        }
    }
}

void KickCmdHandlerHook()
{
    if (*g_pbNetworkGame && *g_pbLocalNetworkGame)
    {
        if (*g_pbDcRun)
        {
            rf::CPlayer *pPlayer;
            
            rf::DcGetArg(DC_ARG_STR, 1);
            pPlayer = FindBestMatchingPlayer(g_pszDcArg);
            if (pPlayer)
            {
                if (pPlayer != *g_ppLocalPlayer)
                {
                    DcPrintf(g_ppszStringsTable[STR_KICKING_PLAYER], pPlayer->strName.psz);
                    KickPlayer(pPlayer);
                } else
                    DcPrintf("You cannot kick yourself!");
            }
        }
        
        if (*g_pbDcHelp)
        {
            DcPrint(g_ppszStringsTable[STR_USAGE], NULL);
            DcPrintf("     kick <%s>", g_ppszStringsTable[STR_PLAYER_NAME]);
        }
    }
}

void UnbanLastCmdHandler(void)
{
    if (*g_pbNetworkGame && *g_pbLocalNetworkGame && *g_pbDcRun)
    {
        BanlistEntry *pEntry = *g_ppBanlistLastEntry;
        if (pEntry != g_pBanlistNullEntry)
        {
            DcPrintf("%s has been unbanned!", pEntry->szIp);
            pEntry->pNext->pPrev = pEntry->pPrev;
            pEntry->pPrev->pNext = pEntry->pPrev;
            RfDelete(pEntry);
        }
    }
}

void InitLazyban(void)
{
    AsmWritter(0x0047B6F0).jmpLong(BanCmdHandlerHook);
    AsmWritter(0x0047B580).jmpLong(KickCmdHandlerHook);
}
