void ReverbSetRoomHfLevel(ReverbContext *pContext, int16_t level){

    LVREV_ControlParams_st    ActiveParams;  
    LVREV_ReturnStatus_en     LvmStatus=LVREV_SUCCESS;  

  
 LvmStatus = LVREV_GetControlParameters(pContext->hInstance, &ActiveParams);
    LVM_ERROR_CHECK(LvmStatus, "LVREV_GetControlParameters", "ReverbSetRoomHfLevel")

 ActiveParams.LPF = ReverbConvertHfLevel(level);

  
 LvmStatus = LVREV_SetControlParameters(pContext->hInstance, &ActiveParams);
    LVM_ERROR_CHECK(LvmStatus, "LVREV_SetControlParameters", "ReverbSetRoomHfLevel")
    pContext->SavedHfLevel = level;
 return;
}
