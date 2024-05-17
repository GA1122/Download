static USHORT DetermineQueueNumber(PARANDIS_ADAPTER *pContext)
{
    if (!pContext->bUsingMSIX)
    {
        DPrintf(0, ("[%s] No MSIX, using 1 queue\n", __FUNCTION__));
        return 1;
    }

    if (pContext->bMultiQueue)
    {
        DPrintf(0, ("[%s] Number of hardware queues = %d\n", __FUNCTION__, pContext->nHardwareQueues));
    }
    else
    {
        DPrintf(0, ("[%s] - CTRL_MQ not acked, # bindles set to 1\n", __FUNCTION__));
        return 1;
    }

    ULONG lnProcessors;
#if NDIS_SUPPORT_NDIS620
    lnProcessors = NdisGroupActiveProcessorCount(ALL_PROCESSOR_GROUPS);
#elif NDIS_SUPPORT_NDIS6
    lnProcessors = NdisSystemProcessorCount();
#else
    lnProcessors = 1;
#endif

    ULONG lnMSIs = (pContext->pMSIXInfoTable->MessageCount - 1) / 2;  

    DPrintf(0, ("[%s] %lu CPUs reported\n", __FUNCTION__, lnProcessors));
    DPrintf(0, ("[%s] %lu MSIs, %lu queues\n", __FUNCTION__, pContext->pMSIXInfoTable->MessageCount, lnMSIs));

    USHORT nMSIs = USHORT(lnMSIs & 0xFFFF);
    USHORT nProcessors = USHORT(lnProcessors & 0xFFFF);

    DPrintf(0, ("[%s] %u CPUs reported\n", __FUNCTION__, nProcessors));
    DPrintf(0, ("[%s] %lu MSIs, %u queues\n", __FUNCTION__, pContext->pMSIXInfoTable->MessageCount, nMSIs));

    USHORT nBundles = (pContext->nHardwareQueues < nProcessors) ? pContext->nHardwareQueues : nProcessors;
    nBundles = (nMSIs < nBundles) ? nMSIs : nBundles;

    DPrintf(0, ("[%s] # of path bundles = %u\n", __FUNCTION__, nBundles));

    return nBundles;
}
