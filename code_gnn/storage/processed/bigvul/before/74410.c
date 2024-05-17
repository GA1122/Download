static void VirtIONetRelease(PARANDIS_ADAPTER *pContext)
{
    BOOLEAN b;
    ULONG i;
    DEBUG_ENTRY(0);

     

    for (i = 0; i < ARRAYSIZE(pContext->ReceiveQueues); i++)
    {
        pRxNetDescriptor pBufferDescriptor;

        while (NULL != (pBufferDescriptor = ReceiveQueueGetBuffer(pContext->ReceiveQueues + i)))
        {
            pBufferDescriptor->Queue->ReuseReceiveBuffer(FALSE, pBufferDescriptor);
        }
    }

    do
    {
        b = pContext->m_upstreamPacketPending != 0;

        if (b)
        {
            DPrintf(0, ("[%s] There are waiting buffers\n", __FUNCTION__));
            PrintStatistics(pContext);
            NdisMSleep(5000000);
        }
    } while (b);

    RestoreMAC(pContext);

    for (i = 0; i < pContext->nPathBundles; i++)
    {
        if (pContext->pPathBundles[i].txCreated)
        {
            pContext->pPathBundles[i].txPath.Shutdown();
        }

        if (pContext->pPathBundles[i].rxCreated)
        {
            pContext->pPathBundles[i].rxPath.Shutdown();

             
            pContext->pPathBundles[i].rxPath.FreeRxDescriptorsFromList();
        }
    }

    if (pContext->bCXPathCreated)
    {
        pContext->CXPath.Shutdown();
    }

    PrintStatistics(pContext);
}
