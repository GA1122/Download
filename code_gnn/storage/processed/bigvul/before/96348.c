static VOID MiniportDisableMSIInterrupt(
    IN PVOID  MiniportInterruptContext,
    IN ULONG  MessageId
    )
{
    PARANDIS_ADAPTER *pContext = (PARANDIS_ADAPTER *)MiniportInterruptContext;
     

    CParaNdisAbstractPath *path = GetPathByMessageId(pContext, MessageId);
    path->DisableInterrupts();
}
