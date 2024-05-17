VOID ParaNdis_FinalizeCleanup(PARANDIS_ADAPTER *pContext)
{
    if (pContext->InterruptHandle)
    {
        NdisMDeregisterInterruptEx(pContext->InterruptHandle);
        pContext->InterruptHandle = NULL;
    }
    if (pContext->BufferListsPool)
    {
        NdisFreeNetBufferListPool(pContext->BufferListsPool);
        pContext->BufferListsPool = NULL;
    }
    if (pContext->DmaHandle)
    {
        NdisMDeregisterScatterGatherDma(pContext->DmaHandle);
        pContext->DmaHandle = NULL;
    }
}
