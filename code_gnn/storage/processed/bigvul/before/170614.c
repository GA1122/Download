void ReverbSetDiffusion(ReverbContext *pContext, int16_t level){

    LVREV_ControlParams_st    ActiveParams;  
    LVREV_ReturnStatus_en     LvmStatus=LVREV_SUCCESS;  

  
 LvmStatus = LVREV_GetControlParameters(pContext->hInstance, &ActiveParams);
    LVM_ERROR_CHECK(LvmStatus, "LVREV_GetControlParameters", "ReverbSetDiffusion")

 ActiveParams.Density = (LVM_INT16)(level/10);

  
 LvmStatus = LVREV_SetControlParameters(pContext->hInstance, &ActiveParams);
    LVM_ERROR_CHECK(LvmStatus, "LVREV_SetControlParameters", "ReverbSetDiffusion")

    pContext->SavedDiffusion = level;
 return;
}
