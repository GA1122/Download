createLppTransposer (HANDLE_SBR_LPP_TRANS hs,  
                     TRANSPOSER_SETTINGS *pSettings,  
 const int  highBandStartSb,  
                     UCHAR *v_k_master,  
 const int numMaster,  
 const int usb,  
 const int timeSlots,  
 const int nCols,  
                     UCHAR *noiseBandTable,  
 const int  noNoiseBands,  
                     UINT   fs,  
 const int chan,  
 const int overlap
 )
{
  
  hs->pSettings = pSettings;

  pSettings->nCols = nCols;
  pSettings->overlap = overlap;

 switch (timeSlots) {

 case 15:
 case 16:
 break;

 default:
 return SBRDEC_UNSUPPORTED_CONFIG;  
 }

 if (chan==0) {
  
    hs->pSettings->nCols = nCols;

 return resetLppTransposer (hs,
                               highBandStartSb,
                               v_k_master,
                               numMaster,
                               noiseBandTable,
                               noNoiseBands,
                               usb,
                               fs);
 }
 return SBRDEC_OK;
}
