int32_t ReverbGetDecayHfRatio(ReverbContext *pContext){

    LVREV_ControlParams_st    ActiveParams;  
    LVREV_ReturnStatus_en     LvmStatus=LVREV_SUCCESS;  

  
 LvmStatus = LVREV_GetControlParameters(pContext->hInstance, &ActiveParams);
    LVM_ERROR_CHECK(LvmStatus, "LVREV_GetControlParameters", "ReverbGetDecayHfRatio")

 if(ActiveParams.Damping != (LVM_INT16)(pContext->SavedDecayHfRatio / 20)){
        ALOGV("\tLVM_ERROR : ReverbGetDecayHfRatio() has wrong level -> %d %d\n",
 ActiveParams.Damping, pContext->SavedDecayHfRatio);
 }

 return pContext->SavedDecayHfRatio;
}
