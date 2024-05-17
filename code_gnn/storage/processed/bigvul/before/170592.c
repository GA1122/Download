int32_t VolumeGetStereoPosition(EffectContext *pContext, int16_t *position){

 LVM_ControlParams_t ActiveParams;  
    LVM_ReturnStatus_en     LvmStatus = LVM_SUCCESS;  
    LVM_INT16               balance;


 LvmStatus = LVM_GetControlParameters(pContext->pBundledContext->hInstance, &ActiveParams);
    LVM_ERROR_CHECK(LvmStatus, "LVM_GetControlParameters", "VolumeGetStereoPosition")
 if(LvmStatus != LVM_SUCCESS) return -EINVAL;


    balance = VolumeConvertStereoPosition(pContext->pBundledContext->positionSaved);

 if(pContext->pBundledContext->bStereoPositionEnabled == LVM_TRUE){
 if(balance != ActiveParams.VC_Balance){
 return -EINVAL;
 }
 }
 *position = (LVM_INT16)pContext->pBundledContext->positionSaved;  
 return 0;
}  
