void BassSetStrength(EffectContext *pContext, uint32_t strength){

    pContext->pBundledContext->BassStrengthSaved = (int)strength;

 LVM_ControlParams_t ActiveParams;  
    LVM_ReturnStatus_en     LvmStatus=LVM_SUCCESS;  

  
 LvmStatus = LVM_GetControlParameters(pContext->pBundledContext->hInstance,
 &ActiveParams);

    LVM_ERROR_CHECK(LvmStatus, "LVM_GetControlParameters", "BassSetStrength")

  
 ActiveParams.BE_EffectLevel    = (LVM_INT16)((15*strength)/1000);
 ActiveParams.BE_CentreFreq     = LVM_BE_CENTRE_90Hz;


  
 LvmStatus = LVM_SetControlParameters(pContext->pBundledContext->hInstance, &ActiveParams);

    LVM_ERROR_CHECK(LvmStatus, "LVM_SetControlParameters", "BassSetStrength")

 LvmEffect_limitLevel(pContext);
}  
