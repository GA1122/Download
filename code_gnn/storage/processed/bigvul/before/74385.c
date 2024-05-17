static void ParaNdis_ResetVirtIONetDevice(PARANDIS_ADAPTER *pContext)
{
    VirtIODeviceReset(pContext->IODevice);
    DPrintf(0, ("[%s] Done\n", __FUNCTION__));
     
    pContext->ulCurrentVlansFilterSet = 0;
}
