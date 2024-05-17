int16_t ReverbGetRoomHfLevel(ReverbContext *pContext){
 int16_t level;

    LVREV_ControlParams_st    ActiveParams;  
    LVREV_ReturnStatus_en     LvmStatus=LVREV_SUCCESS;  

  
 LvmStatus = LVREV_GetControlParameters(pContext->hInstance, &ActiveParams);
    LVM_ERROR_CHECK(LvmStatus, "LVREV_GetControlParameters", "ReverbGetRoomHfLevel")

    level = ReverbConvertHfLevel(pContext->SavedHfLevel);


 if(ActiveParams.LPF != level){
        ALOGV("\tLVM_ERROR : (ignore at start up) ReverbGetRoomHfLevel() has wrong level -> %d %d\n",
 ActiveParams.Level, level);
 }

 return pContext->SavedHfLevel;
}
