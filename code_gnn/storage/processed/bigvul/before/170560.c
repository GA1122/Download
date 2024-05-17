int Effect_process(effect_handle_t     self,
 audio_buffer_t *inBuffer,
 audio_buffer_t *outBuffer){
 EffectContext * pContext = (EffectContext *) self;
    LVM_ReturnStatus_en     LvmStatus = LVM_SUCCESS;  
 int    status = 0;
 int    lvmStatus = 0;
    LVM_INT16   *in  = (LVM_INT16 *)inBuffer->raw;
    LVM_INT16   *out = (LVM_INT16 *)outBuffer->raw;


 if (pContext == NULL){
        ALOGV("\tLVM_ERROR : Effect_process() ERROR pContext == NULL");
 return -EINVAL;
 }


 if (inBuffer == NULL  || inBuffer->raw == NULL  ||
            outBuffer == NULL || outBuffer->raw == NULL ||
            inBuffer->frameCount != outBuffer->frameCount){
        ALOGV("\tLVM_ERROR : Effect_process() ERROR NULL INPUT POINTER OR FRAME COUNT IS WRONG");
 return -EINVAL;
 }
 if ((pContext->pBundledContext->bBassEnabled == LVM_FALSE)&&
 (pContext->EffectType == LVM_BASS_BOOST)){
 if(pContext->pBundledContext->SamplesToExitCountBb > 0){
            pContext->pBundledContext->SamplesToExitCountBb -= outBuffer->frameCount * 2;  
 }
 if(pContext->pBundledContext->SamplesToExitCountBb <= 0) {
            status = -ENODATA;
            pContext->pBundledContext->NumberEffectsEnabled--;
            ALOGV("\tEffect_process() this is the last frame for LVM_BASS_BOOST");
 }
 }
 if ((pContext->pBundledContext->bVolumeEnabled == LVM_FALSE)&&
 (pContext->EffectType == LVM_VOLUME)){
        status = -ENODATA;
        pContext->pBundledContext->NumberEffectsEnabled--;
 }
 if ((pContext->pBundledContext->bEqualizerEnabled == LVM_FALSE)&&
 (pContext->EffectType == LVM_EQUALIZER)){
 if(pContext->pBundledContext->SamplesToExitCountEq > 0){
            pContext->pBundledContext->SamplesToExitCountEq -= outBuffer->frameCount * 2;  
 }
 if(pContext->pBundledContext->SamplesToExitCountEq <= 0) {
            status = -ENODATA;
            pContext->pBundledContext->NumberEffectsEnabled--;
            ALOGV("\tEffect_process() this is the last frame for LVM_EQUALIZER");
 }
 }
 if ((pContext->pBundledContext->bVirtualizerEnabled == LVM_FALSE)&&
 (pContext->EffectType == LVM_VIRTUALIZER)){
 if(pContext->pBundledContext->SamplesToExitCountVirt > 0){
            pContext->pBundledContext->SamplesToExitCountVirt -= outBuffer->frameCount * 2; 
 }
 if(pContext->pBundledContext->SamplesToExitCountVirt <= 0) {
            status = -ENODATA;
            pContext->pBundledContext->NumberEffectsEnabled--;
            ALOGV("\tEffect_process() this is the last frame for LVM_VIRTUALIZER");
 }
 }

 if(status != -ENODATA){
        pContext->pBundledContext->NumberEffectsCalled++;
 }

 if(pContext->pBundledContext->NumberEffectsCalled ==
       pContext->pBundledContext->NumberEffectsEnabled){

 if(status == -ENODATA){
            ALOGV("\tEffect_process() processing last frame");
 }
        pContext->pBundledContext->NumberEffectsCalled = 0;
  
        lvmStatus = android::LvmBundle_process( (LVM_INT16 *)inBuffer->raw,
 (LVM_INT16 *)outBuffer->raw,
                                                outBuffer->frameCount,
                                                pContext);
 if(lvmStatus != LVM_SUCCESS){
            ALOGV("\tLVM_ERROR : LvmBundle_process returned error %d", lvmStatus);
 return lvmStatus;
 }
 } else {
 if (pContext->config.outputCfg.accessMode == EFFECT_BUFFER_ACCESS_ACCUMULATE) {
 for (size_t i=0; i < outBuffer->frameCount*2; i++){
                outBuffer->s16[i] =
                        clamp16((LVM_INT32)outBuffer->s16[i] + (LVM_INT32)inBuffer->s16[i]);
 }
 } else if (outBuffer->raw != inBuffer->raw) {
            memcpy(outBuffer->raw, inBuffer->raw, outBuffer->frameCount*sizeof(LVM_INT16)*2);
 }
 }

 return status;
}  
