VOID ParaNdis_PowerOff(PARANDIS_ADAPTER *pContext)
{
    DEBUG_ENTRY(0);
    ParaNdis_DebugHistory(pContext, hopPowerOff, NULL, 1, 0, 0);

    pContext->bConnected = FALSE;

    pContext->bFastSuspendInProcess = pContext->bNoPauseOnSuspend && pContext->ReceiveState == srsEnabled;
    ParaNdis_Suspend(pContext);

    ParaNdis_RemoveDriverOKStatus(pContext);
    
    if (pContext->bFastSuspendInProcess)
    {
        InterlockedExchange(&pContext->ReuseBufferRegular, FALSE);
    }
    
#if !NDIS_SUPPORT_NDIS620
    ParaNdis_SetLinkState(pContext, MediaConnectStateDisconnected);
#endif
    ParaNdis_SetPowerState(pContext, NdisDeviceStateD3);
    ParaNdis_SetLinkState(pContext, MediaConnectStateUnknown);

    PreventDPCServicing(pContext);

     

    for (UINT i = 0; i < pContext->nPathBundles; i++)
    {
        pContext->pPathBundles[i].txPath.Shutdown();
        pContext->pPathBundles[i].rxPath.Shutdown();
    }

    if (pContext->bCXPathCreated)
    {
        pContext->CXPath.Shutdown();
    }

    ParaNdis_ResetVirtIONetDevice(pContext);
    ParaNdis_DebugHistory(pContext, hopPowerOff, NULL, 0, 0, 0);
}
