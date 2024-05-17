BOOLEAN ParaNdis_CheckForHang(PARANDIS_ADAPTER *pContext)
{
    static int nHangOn = 0;
    BOOLEAN b = nHangOn >= 3 && nHangOn < 6;
    DEBUG_ENTRY(3);
    b |= CheckRunningDpc(pContext);
    DEBUG_EXIT_STATUS(b ? 0 : 6, b);
    return b;
}
