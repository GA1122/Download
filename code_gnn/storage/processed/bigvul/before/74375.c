VOID ParaNdis_OnShutdown(PARANDIS_ADAPTER *pContext)
{
    DEBUG_ENTRY(0);  
    ParaNdis_ResetVirtIONetDevice(pContext);
}
