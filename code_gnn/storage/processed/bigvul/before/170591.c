int32_t VolumeEnableStereoPosition(EffectContext *pContext, uint32_t enabled){

    pContext->pBundledContext->bStereoPositionEnabled = enabled;

 LVM_ControlParams_t ActiveParams;  
    LVM_ReturnStatus_en     LvmStatus=LVM_SUCCESS;  

  
 LvmStatus = LVM_GetControlParameters(pContext->pBundledContext->hInstance, &ActiveParams);
    LVM_ERROR_CHECK(LvmStatus, "LVM_GetControlParameters", "VolumeEnableStereoPosition")
 if(LvmStatus != LVM_SUCCESS) return -EINVAL;


  
 if(pContext->pBundledContext->bStereoPositionEnabled == LVM_FALSE){
 ActiveParams.VC_Balance = 0;
 }else{
 ActiveParams.VC_Balance  =
 VolumeConvertStereoPosition(pContext->pBundledContext->positionSaved);
 }

  
 LvmStatus = LVM_SetControlParameters(pContext->pBundledContext->hInstance, &ActiveParams);
    LVM_ERROR_CHECK(LvmStatus, "LVM_SetControlParameters", "VolumeEnableStereoPosition")
 if(LvmStatus != LVM_SUCCESS) return -EINVAL;

 return 0;
}  
