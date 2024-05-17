ar6000_cac_event(struct ar6_softc *ar, u8 ac, u8 cacIndication,
                 u8 statusCode, u8 *tspecSuggestion)
{
    WMM_TSPEC_IE    *tspecIe;

     
    tspecIe = (WMM_TSPEC_IE *)tspecSuggestion;

     
    A_PRINTF("AR6000 CAC notification. "
                "AC = %d, cacIndication = 0x%x, statusCode = 0x%x\n",
                 ac, cacIndication, statusCode);
}
