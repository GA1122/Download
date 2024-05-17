int32_t ReverbGetDiffusion(ReverbContext *pContext){

    LVREV_ControlParams_st    ActiveParams;  
    LVREV_ReturnStatus_en     LvmStatus=LVREV_SUCCESS;  
    LVM_INT16                 Temp;

  
 LvmStatus = LVREV_GetControlParameters(pContext->hInstance, &ActiveParams);
    LVM_ERROR_CHECK(LvmStatus, "LVREV_GetControlParameters", "ReverbGetDiffusion")

 Temp = (LVM_INT16)(pContext->SavedDiffusion/10);

 if(ActiveParams.Density != Temp){
        ALOGV("\tLVM_ERROR : ReverbGetDiffusion invalid value %d %d", Temp, ActiveParams.Density);
 }

 return pContext->SavedDiffusion;
}
