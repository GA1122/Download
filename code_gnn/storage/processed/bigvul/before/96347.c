static VOID MiniportDisableInterruptEx(IN PVOID MiniportInterruptContext)
{
    DEBUG_ENTRY(0);
    PARANDIS_ADAPTER *pContext = (PARANDIS_ADAPTER *)MiniportInterruptContext;

     
    for (UINT i = 0; i < pContext->nPathBundles; i++)
    {
        pContext->pPathBundles[i].txPath.DisableInterrupts();
        pContext->pPathBundles[i].rxPath.DisableInterrupts();
    }
    if (pContext->bCXPathCreated)
    {
        pContext->CXPath.DisableInterrupts();
    }
}
