uint32_t VirtualizerGetStrength(EffectContext *pContext){

 LVM_ControlParams_t ActiveParams;  
    LVM_ReturnStatus_en     LvmStatus = LVM_SUCCESS;  

 LvmStatus = LVM_GetControlParameters(pContext->pBundledContext->hInstance, &ActiveParams);

    LVM_ERROR_CHECK(LvmStatus, "LVM_GetControlParameters", "VirtualizerGetStrength")
 if(LvmStatus != LVM_SUCCESS) return -EINVAL;

 return pContext->pBundledContext->VirtStrengthSaved;
}  
