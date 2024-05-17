int LvmEffect_disable(EffectContext *pContext){

 LVM_ControlParams_t ActiveParams;  
    LVM_ReturnStatus_en     LvmStatus = LVM_SUCCESS;  
  
 LvmStatus = LVM_GetControlParameters(pContext->pBundledContext->hInstance,
 &ActiveParams);

    LVM_ERROR_CHECK(LvmStatus, "LVM_GetControlParameters", "LvmEffect_disable")
 if(LvmStatus != LVM_SUCCESS) return -EINVAL;

 if(pContext->EffectType == LVM_BASS_BOOST) {
        ALOGV("\tLvmEffect_disable : Disabling LVM_BASS_BOOST");
 ActiveParams.BE_OperatingMode       = LVM_BE_OFF;
 }
 if(pContext->EffectType == LVM_VIRTUALIZER) {
        ALOGV("\tLvmEffect_disable : Disabling LVM_VIRTUALIZER");
 ActiveParams.VirtualizerOperatingMode = LVM_MODE_OFF;
 }
 if(pContext->EffectType == LVM_EQUALIZER) {
        ALOGV("\tLvmEffect_disable : Disabling LVM_EQUALIZER");
 ActiveParams.EQNB_OperatingMode     = LVM_EQNB_OFF;
 }
 if(pContext->EffectType == LVM_VOLUME) {
        ALOGV("\tLvmEffect_disable : Disabling LVM_VOLUME");
 }

 LvmStatus = LVM_SetControlParameters(pContext->pBundledContext->hInstance, &ActiveParams);
    LVM_ERROR_CHECK(LvmStatus, "LVM_SetControlParameters", "LvmEffect_disable")
 if(LvmStatus != LVM_SUCCESS) return -EINVAL;

 LvmEffect_limitLevel(pContext);
 return 0;
}
