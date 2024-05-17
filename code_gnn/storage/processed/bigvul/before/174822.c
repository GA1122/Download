int32_t VolumeSetMute(EffectContext *pContext, uint32_t mute){

    pContext->pBundledContext->bMuteEnabled = mute;

  
 if(pContext->pBundledContext->bMuteEnabled == LVM_TRUE){
        pContext->pBundledContext->levelSaved = pContext->pBundledContext->volume;
        pContext->pBundledContext->volume = -96;
 }else{
        pContext->pBundledContext->volume = pContext->pBundledContext->levelSaved;
 }

 EqualizerLimitBandLevels(pContext);

 return 0;
}  
