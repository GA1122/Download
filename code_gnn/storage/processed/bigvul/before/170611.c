void ReverbSetDecayHfRatio(ReverbContext *pContext, int16_t ratio){

    LVREV_ControlParams_st    ActiveParams;  
    LVREV_ReturnStatus_en     LvmStatus=LVREV_SUCCESS;  

  
 LvmStatus = LVREV_GetControlParameters(pContext->hInstance, &ActiveParams);
    LVM_ERROR_CHECK(LvmStatus, "LVREV_GetControlParameters", "ReverbSetDecayHfRatio")

 ActiveParams.Damping = (LVM_INT16)(ratio/20);

  
 LvmStatus = LVREV_SetControlParameters(pContext->hInstance, &ActiveParams);
    LVM_ERROR_CHECK(LvmStatus, "LVREV_SetControlParameters", "ReverbSetDecayHfRatio")

    pContext->SavedDecayHfRatio = ratio;
 return;
}
