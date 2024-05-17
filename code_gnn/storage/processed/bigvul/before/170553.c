uint32_t BassGetStrength(EffectContext *pContext){

 LVM_ControlParams_t ActiveParams;  
    LVM_ReturnStatus_en     LvmStatus = LVM_SUCCESS;  
  
 LvmStatus = LVM_GetControlParameters(pContext->pBundledContext->hInstance,
 &ActiveParams);

    LVM_ERROR_CHECK(LvmStatus, "LVM_GetControlParameters", "BassGetStrength")
 if(LvmStatus != LVM_SUCCESS) return -EINVAL;


  
 if(ActiveParams.BE_EffectLevel !=
 (LVM_INT16)((15*pContext->pBundledContext->BassStrengthSaved)/1000)){
        ALOGV("\tLVM_ERROR : BassGetStrength module strength does not match savedStrength %d %d\n",
 ActiveParams.BE_EffectLevel, pContext->pBundledContext->BassStrengthSaved);
 return -EINVAL;
 }

 return pContext->pBundledContext->BassStrengthSaved;
}  
