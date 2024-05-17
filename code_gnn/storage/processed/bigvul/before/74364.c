VOID ParaNdis_CleanupContext(PARANDIS_ADAPTER *pContext)
{
     
    if (pContext->IODevice->addr)
    {
        if (pContext->bDeviceInitialized)
        {
            ParaNdis_RemoveDriverOKStatus(pContext);
        }
    }

    PreventDPCServicing(pContext);

     

    if (pContext->IODevice->addr)
    {
        ParaNdis_ResetVirtIONetDevice(pContext);
    }

    ParaNdis_SetPowerState(pContext, NdisDeviceStateD3);
    ParaNdis_SetLinkState(pContext, MediaConnectStateUnknown);
    VirtIONetRelease(pContext);

    ParaNdis_FinalizeCleanup(pContext);

    if (pContext->ReceiveQueuesInitialized)
    {
        ULONG i;

        for(i = 0; i < ARRAYSIZE(pContext->ReceiveQueues); i++)
        {
            NdisFreeSpinLock(&pContext->ReceiveQueues[i].Lock);
        }
    }

    pContext->m_PauseLock.~CNdisRWLock();

#if PARANDIS_SUPPORT_RSS
    if (pContext->bRSSInitialized)
    {
        ParaNdis6_RSSCleanupConfiguration(&pContext->RSSParameters);
    }

    pContext->RSSParameters.rwLock.~CNdisRWLock();
#endif

    if (pContext->bCXPathAllocated)
    {
        pContext->CXPath.~CParaNdisCX();
        pContext->bCXPathAllocated = false;
    }

    if (pContext->pPathBundles != NULL)
    {
        USHORT i;

        for (i = 0; i < pContext->nPathBundles; i++)
        {
            pContext->pPathBundles[i].~CPUPathesBundle();
        }
        NdisFreeMemoryWithTagPriority(pContext->MiniportHandle, pContext->pPathBundles, PARANDIS_MEMORY_TAG);
        pContext->pPathBundles = nullptr;
    }

    if (pContext->RSS2QueueMap)
    {
        NdisFreeMemoryWithTagPriority(pContext->MiniportHandle, pContext->RSS2QueueMap, PARANDIS_MEMORY_TAG);
        pContext->RSS2QueueMap = nullptr;
        pContext->RSS2QueueLength = 0;
    }

    if (pContext->IODevice)
    {
        NdisFreeMemoryWithTagPriority(pContext->MiniportHandle, pContext->IODevice, PARANDIS_MEMORY_TAG);
        pContext->IODevice = nullptr;
    }

    if (pContext->AdapterResources.ulIOAddress)
    {
        NdisMDeregisterIoPortRange(
            pContext->MiniportHandle,
            pContext->AdapterResources.ulIOAddress,
            pContext->AdapterResources.IOLength,
            pContext->pIoPortOffset);
        pContext->AdapterResources.ulIOAddress = 0;
    }
}