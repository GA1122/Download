int VolumeSetStereoPosition(EffectContext *pContext, int16_t position){

 LVM_ControlParams_t ActiveParams;  
    LVM_ReturnStatus_en     LvmStatus=LVM_SUCCESS;  
    LVM_INT16               Balance = 0;



    pContext->pBundledContext->positionSaved = position;
 Balance = VolumeConvertStereoPosition(pContext->pBundledContext->positionSaved);


 if(pContext->pBundledContext->bStereoPositionEnabled == LVM_TRUE){

        pContext->pBundledContext->positionSaved = position;
  
 LvmStatus = LVM_GetControlParameters(pContext->pBundledContext->hInstance, &ActiveParams);
        LVM_ERROR_CHECK(LvmStatus, "LVM_GetControlParameters", "VolumeSetStereoPosition")
 if(LvmStatus != LVM_SUCCESS) return -EINVAL;

  
 ActiveParams.VC_Balance  = Balance;

  
 LvmStatus = LVM_SetControlParameters(pContext->pBundledContext->hInstance, &ActiveParams);
        LVM_ERROR_CHECK(LvmStatus, "LVM_SetControlParameters", "VolumeSetStereoPosition")
 if(LvmStatus != LVM_SUCCESS) return -EINVAL;


  
 LvmStatus = LVM_GetControlParameters(pContext->pBundledContext->hInstance, &ActiveParams);
        LVM_ERROR_CHECK(LvmStatus, "LVM_GetControlParameters", "VolumeSetStereoPosition")
 if(LvmStatus != LVM_SUCCESS) return -EINVAL;
 }
 else{
 }
 return 0;
}  
