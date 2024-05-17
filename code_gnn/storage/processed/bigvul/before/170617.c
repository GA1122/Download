void ReverbSetRoomLevel(ReverbContext *pContext, int16_t level){

    LVREV_ControlParams_st    ActiveParams;  
    LVREV_ReturnStatus_en     LvmStatus=LVREV_SUCCESS;  
    LVM_INT32                 CombinedLevel;  

  
 LvmStatus = LVREV_GetControlParameters(pContext->hInstance, &ActiveParams);
    LVM_ERROR_CHECK(LvmStatus, "LVREV_GetControlParameters", "ReverbSetRoomLevel")

 CombinedLevel = (level + pContext->SavedReverbLevel)-LVREV_MAX_REVERB_LEVEL;
 ActiveParams.Level = ReverbConvertLevel(CombinedLevel);

  
 LvmStatus = LVREV_SetControlParameters(pContext->hInstance, &ActiveParams);
    LVM_ERROR_CHECK(LvmStatus, "LVREV_SetControlParameters", "ReverbSetRoomLevel")

    pContext->SavedRoomLevel = level;
 return;
}
