VOID ParaNdis_PowerOn(PARANDIS_ADAPTER *pContext)
{
    UINT i;

    DEBUG_ENTRY(0);
    ParaNdis_DebugHistory(pContext, hopPowerOn, NULL, 1, 0, 0);
    ParaNdis_ResetVirtIONetDevice(pContext);
    VirtIODeviceAddStatus(pContext->IODevice, VIRTIO_CONFIG_S_ACKNOWLEDGE | VIRTIO_CONFIG_S_DRIVER);
     
    VirtIODeviceReadHostFeatures(pContext->IODevice);
    VirtIODeviceWriteGuestFeatures(pContext->IODevice, pContext->u32GuestFeatures);

    for (i = 0; i < pContext->nPathBundles; i++)
    {
        pContext->pPathBundles[i].txPath.Renew();
        pContext->pPathBundles[i].rxPath.Renew();
    }
    if (pContext->bCXPathCreated)
    {
        pContext->CXPath.Renew();
    }

    ParaNdis_RestoreDeviceConfigurationAfterReset(pContext);

    ParaNdis_UpdateDeviceFilters(pContext);
    ParaNdis_UpdateMAC(pContext);

    InterlockedExchange(&pContext->ReuseBufferRegular, TRUE);
    
    for (i = 0; i < pContext->nPathBundles; i++)
    {
        pContext->pPathBundles[i].rxPath.PopulateQueue();
    }

    ReadLinkState(pContext);
    ParaNdis_SetPowerState(pContext, NdisDeviceStateD0);
    ParaNdis_SynchronizeLinkState(pContext);
    pContext->bEnableInterruptHandlingDPC = TRUE;
    ParaNdis_AddDriverOKStatus(pContext);

    ParaNdis_Resume(pContext);
    pContext->bFastSuspendInProcess = FALSE;

    ParaNdis_DebugHistory(pContext, hopPowerOn, NULL, 0, 0, 0);
}
