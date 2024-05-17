uint32_t ReverbGetDecayTime(ReverbContext *pContext){

    LVREV_ControlParams_st    ActiveParams;  
    LVREV_ReturnStatus_en     LvmStatus=LVREV_SUCCESS;  

  
 LvmStatus = LVREV_GetControlParameters(pContext->hInstance, &ActiveParams);
    LVM_ERROR_CHECK(LvmStatus, "LVREV_GetControlParameters", "ReverbGetDecayTime")

 if(ActiveParams.T60 != pContext->SavedDecayTime){
        ALOGV("\tLVM_ERROR : ReverbGetDecayTime() has wrong level -> %d %d\n",
 ActiveParams.T60, pContext->SavedDecayTime);
 }

 return (uint32_t)ActiveParams.T60;
}
