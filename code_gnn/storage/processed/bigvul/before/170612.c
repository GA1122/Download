void ReverbSetDecayTime(ReverbContext *pContext, uint32_t time){

    LVREV_ControlParams_st    ActiveParams;  
    LVREV_ReturnStatus_en     LvmStatus=LVREV_SUCCESS;  

  
 LvmStatus = LVREV_GetControlParameters(pContext->hInstance, &ActiveParams);
    LVM_ERROR_CHECK(LvmStatus, "LVREV_GetControlParameters", "ReverbSetDecayTime")

 if (time <= LVREV_MAX_T60) {
 ActiveParams.T60 = (LVM_UINT16)time;
 }
 else {
 ActiveParams.T60 = LVREV_MAX_T60;
 }

  
 LvmStatus = LVREV_SetControlParameters(pContext->hInstance, &ActiveParams);
    LVM_ERROR_CHECK(LvmStatus, "LVREV_SetControlParameters", "ReverbSetDecayTime")

    pContext->SamplesToExitCount = (ActiveParams.T60 * pContext->config.inputCfg.samplingRate)/1000;
    pContext->SavedDecayTime = (int16_t)time;
 return;
}
