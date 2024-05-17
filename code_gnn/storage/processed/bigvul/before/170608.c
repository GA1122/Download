int16_t ReverbGetReverbLevel(ReverbContext *pContext){
 int16_t level;

    LVREV_ControlParams_st    ActiveParams;  
    LVREV_ReturnStatus_en     LvmStatus=LVREV_SUCCESS;  
    LVM_INT32                 CombinedLevel;  

  
 LvmStatus = LVREV_GetControlParameters(pContext->hInstance, &ActiveParams);
    LVM_ERROR_CHECK(LvmStatus, "LVREV_GetControlParameters", "ReverbGetReverbLevel")

 CombinedLevel = (pContext->SavedReverbLevel + pContext->SavedRoomLevel)-LVREV_MAX_REVERB_LEVEL;

    level = ReverbConvertLevel(CombinedLevel);


 if(ActiveParams.Level != level){
        ALOGV("\tLVM_ERROR : (ignore at start up) ReverbGetReverbLevel() has wrong level -> %d %d\n",
 ActiveParams.Level, level);
 }


 return pContext->SavedReverbLevel;
}
