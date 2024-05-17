static VOID MiniportInterruptDPC(
    IN NDIS_HANDLE  MiniportInterruptContext,
    IN PVOID  MiniportDpcContext,
    IN PVOID                   ReceiveThrottleParameters,
    IN PVOID                   NdisReserved2
    )
{
    PARANDIS_ADAPTER *pContext = (PARANDIS_ADAPTER *)MiniportInterruptContext;
    bool requiresDPCRescheduling;

#if NDIS_SUPPORT_NDIS620
    PNDIS_RECEIVE_THROTTLE_PARAMETERS RxThrottleParameters = (PNDIS_RECEIVE_THROTTLE_PARAMETERS)ReceiveThrottleParameters;
    DEBUG_ENTRY(5);
    RxThrottleParameters->MoreNblsPending = 0;
    requiresDPCRescheduling = ParaNdis_DPCWorkBody(pContext, RxThrottleParameters->MaxNblsToIndicate);
    if (requiresDPCRescheduling)
        {
            GROUP_AFFINITY Affinity;
            GetAffinityForCurrentCpu(&Affinity);

            NdisMQueueDpcEx(pContext->InterruptHandle, 0, &Affinity, MiniportDpcContext);
        }
#else  
    DEBUG_ENTRY(5);
    UNREFERENCED_PARAMETER(ReceiveThrottleParameters);

    requiresDPCRescheduling = ParaNdis_DPCWorkBody(pContext, PARANDIS_UNLIMITED_PACKETS_TO_INDICATE);
    if (requiresDPCRescheduling)
    {
        DPrintf(4, ("[%s] Queued additional DPC for %d\n", __FUNCTION__,  requiresDPCRescheduling));
        NdisMQueueDpc(pContext->InterruptHandle, 0, 1 << KeGetCurrentProcessorNumber(), MiniportDpcContext);
    }
#endif  

    UNREFERENCED_PARAMETER(NdisReserved2);
}