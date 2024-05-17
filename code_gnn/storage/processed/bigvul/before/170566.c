int32_t EqualizerGetCentreFrequency(EffectContext *pContext, int32_t band){
 int32_t Frequency =0;

 LVM_ControlParams_t ActiveParams;  
    LVM_ReturnStatus_en     LvmStatus = LVM_SUCCESS;  
 LVM_EQNB_BandDef_t *BandDef;
  
 LvmStatus = LVM_GetControlParameters(pContext->pBundledContext->hInstance,
 &ActiveParams);

    LVM_ERROR_CHECK(LvmStatus, "LVM_GetControlParameters", "EqualizerGetCentreFrequency")

 BandDef = ActiveParams.pEQNB_BandDefinition;
 Frequency = (int32_t)BandDef[band].Frequency*1000;  

 return Frequency;
}
