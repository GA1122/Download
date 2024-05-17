VOID ParaNdis_OnPnPEvent(
    PARANDIS_ADAPTER *pContext,
    NDIS_DEVICE_PNP_EVENT pEvent,
    PVOID   pInfo,
    ULONG   ulSize)
{
    const char *pName = "";

    UNREFERENCED_PARAMETER(pInfo);
    UNREFERENCED_PARAMETER(ulSize);

    DEBUG_ENTRY(0);
#undef MAKECASE
#define MAKECASE(x) case (x): pName = #x; break;
    switch (pEvent)
    {
        MAKECASE(NdisDevicePnPEventQueryRemoved)
        MAKECASE(NdisDevicePnPEventRemoved)
        MAKECASE(NdisDevicePnPEventSurpriseRemoved)
        MAKECASE(NdisDevicePnPEventQueryStopped)
        MAKECASE(NdisDevicePnPEventStopped)
        MAKECASE(NdisDevicePnPEventPowerProfileChanged)
        MAKECASE(NdisDevicePnPEventFilterListChanged)
        default:
            break;
    }
    ParaNdis_DebugHistory(pContext, hopPnpEvent, NULL, pEvent, 0, 0);
    DPrintf(0, ("[%s] (%s)\n", __FUNCTION__, pName));
    if (pEvent == NdisDevicePnPEventSurpriseRemoved)
    {
        pContext->bSurprizeRemoved = TRUE;
        ParaNdis_ResetVirtIONetDevice(pContext);
        {
            UINT i;

            for (i = 0; i < pContext->nPathBundles; i++)
            {
                pContext->pPathBundles[i].txPath.Pause();
            }
        }
    }
    pContext->PnpEvents[pContext->nPnpEventIndex++] = pEvent;
    if (pContext->nPnpEventIndex > sizeof(pContext->PnpEvents)/sizeof(pContext->PnpEvents[0]))
        pContext->nPnpEventIndex = 0;
}