int LvmEffect_enable(EffectContext *pContext){

 LVM_ControlParams_t ActiveParams;  
    LVM_ReturnStatus_en     LvmStatus = LVM_SUCCESS;  

  
 LvmStatus = LVM_GetControlParameters(pContext->pBundledContext->hInstance,
 &ActiveParams);

    LVM_ERROR_CHECK(LvmStatus, "LVM_GetControlParameters", "LvmEffect_enable")
 if(LvmStatus != LVM_SUCCESS) return -EINVAL;

 if(pContext->EffectType == LVM_BASS_BOOST) {
        ALOGV("\tLvmEffect_enable : Enabling LVM_BASS_BOOST");
 ActiveParams.BE_OperatingMode       = LVM_BE_ON;
 }
 if(pContext->EffectType == LVM_VIRTUALIZER) {
        ALOGV("\tLvmEffect_enable : Enabling LVM_VIRTUALIZER");
 ActiveParams.VirtualizerOperatingMode = LVM_MODE_ON;
 }
 if(pContext->EffectType == LVM_EQUALIZER) {
        ALOGV("\tLvmEffect_enable : Enabling LVM_EQUALIZER");
 ActiveParams.EQNB_OperatingMode     = LVM_EQNB_ON;
 }
 if(pContext->EffectType == LVM_VOLUME) {
        ALOGV("\tLvmEffect_enable : Enabling LVM_VOLUME");
 }

 LvmStatus = LVM_SetControlParameters(pContext->pBundledContext->hInstance, &ActiveParams);
    LVM_ERROR_CHECK(LvmStatus, "LVM_SetControlParameters", "LvmEffect_enable")
 if(LvmStatus != LVM_SUCCESS) return -EINVAL;

 return 0;
}
