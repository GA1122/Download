void ReverbSetReverbLevel(ReverbContext *pContext, int16_t level){

    LVREV_ControlParams_st    ActiveParams;  
    LVREV_ReturnStatus_en     LvmStatus=LVREV_SUCCESS;  
    LVM_INT32                 CombinedLevel;  

  
 LvmStatus = LVREV_GetControlParameters(pContext->hInstance, &ActiveParams);
    LVM_ERROR_CHECK(LvmStatus, "LVREV_GetControlParameters", "ReverbSetReverbLevel")

 CombinedLevel = (level + pContext->SavedRoomLevel)-LVREV_MAX_REVERB_LEVEL;

 ActiveParams.Level = ReverbConvertLevel(CombinedLevel);


  
 LvmStatus = LVREV_SetControlParameters(pContext->hInstance, &ActiveParams);
    LVM_ERROR_CHECK(LvmStatus, "LVREV_SetControlParameters", "ReverbSetReverbLevel")

    pContext->SavedReverbLevel = level;
 return;
}
