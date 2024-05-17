ar6000_reportError_event(struct ar6_softc *ar, WMI_TARGET_ERROR_VAL errorVal)
{
	static const char * const errString[] = {
		[WMI_TARGET_PM_ERR_FAIL]    "WMI_TARGET_PM_ERR_FAIL",
		[WMI_TARGET_KEY_NOT_FOUND]  "WMI_TARGET_KEY_NOT_FOUND",
		[WMI_TARGET_DECRYPTION_ERR] "WMI_TARGET_DECRYPTION_ERR",
		[WMI_TARGET_BMISS]          "WMI_TARGET_BMISS",
		[WMI_PSDISABLE_NODE_JOIN]   "WMI_PSDISABLE_NODE_JOIN"
	};

    A_PRINTF("AR6000 Error on Target. Error = 0x%x\n", errorVal);

     
    if(errorVal & (errorVal - 1))
       return;

    A_PRINTF("AR6000 Error type = ");
    switch(errorVal)
    {
        case WMI_TARGET_PM_ERR_FAIL:
        case WMI_TARGET_KEY_NOT_FOUND:
        case WMI_TARGET_DECRYPTION_ERR:
        case WMI_TARGET_BMISS:
        case WMI_PSDISABLE_NODE_JOIN:
            A_PRINTF("%s\n", errString[errorVal]);
            break;
        default:
            A_PRINTF("INVALID\n");
            break;
    }

}