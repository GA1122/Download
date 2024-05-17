void VirtualizerSetStrength(EffectContext *pContext, uint32_t strength){
 LVM_ControlParams_t ActiveParams;  
    LVM_ReturnStatus_en     LvmStatus=LVM_SUCCESS;  

    pContext->pBundledContext->VirtStrengthSaved = (int)strength;

  
 LvmStatus = LVM_GetControlParameters(pContext->pBundledContext->hInstance,&ActiveParams);

    LVM_ERROR_CHECK(LvmStatus, "LVM_GetControlParameters", "VirtualizerSetStrength")

  
 ActiveParams.CS_EffectLevel             = (int)((strength*32767)/1000);


  
 LvmStatus = LVM_SetControlParameters(pContext->pBundledContext->hInstance, &ActiveParams);
    LVM_ERROR_CHECK(LvmStatus, "LVM_SetControlParameters", "VirtualizerSetStrength")
}  
