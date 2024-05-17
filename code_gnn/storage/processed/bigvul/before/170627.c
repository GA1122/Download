int process( LVM_INT16     *pIn,
             LVM_INT16     *pOut,
 int           frameCount,
 ReverbContext *pContext){

    LVM_INT16               samplesPerFrame = 1;
    LVREV_ReturnStatus_en   LvmStatus = LVREV_SUCCESS;  
    LVM_INT16 *OutFrames16;


 if (pContext->config.inputCfg.channels == AUDIO_CHANNEL_OUT_STEREO) {
        samplesPerFrame = 2;
 } else if (pContext->config.inputCfg.channels != AUDIO_CHANNEL_OUT_MONO) {
        ALOGV("\tLVREV_ERROR : process invalid PCM format");
 return -EINVAL;
 }

 OutFrames16 = (LVM_INT16 *)pContext->OutFrames32;

 if((pContext->InFrames32 == NULL)||(pContext->OutFrames32 == NULL)){
        ALOGV("\tLVREV_ERROR : process failed to allocate memory for temporary buffers ");
 return -EINVAL;
 }

 #ifdef LVM_PCM
    fwrite(pIn, frameCount*sizeof(LVM_INT16)*samplesPerFrame, 1, pContext->PcmInPtr);
    fflush(pContext->PcmInPtr);
 #endif

 if (pContext->preset && pContext->nextPreset != pContext->curPreset) {
 Reverb_LoadPreset(pContext);
 }



 if (pContext->auxiliary) {
 for(int i=0; i<frameCount*samplesPerFrame; i++){
            pContext->InFrames32[i] = (LVM_INT32)pIn[i]<<8;
 }
 } else {
 for (int i = 0; i < frameCount; i++) {
            pContext->InFrames32[2*i] = (pIn[2*i] * REVERB_SEND_LEVEL) >> 4;  
            pContext->InFrames32[2*i+1] = (pIn[2*i+1] * REVERB_SEND_LEVEL) >> 4;  
 }
 }

 if (pContext->preset && pContext->curPreset == REVERB_PRESET_NONE) {
        memset(pContext->OutFrames32, 0, frameCount * sizeof(LVM_INT32) * 2);  
 } else {
 if(pContext->bEnabled == LVM_FALSE && pContext->SamplesToExitCount > 0) {
            memset(pContext->InFrames32,0,frameCount * sizeof(LVM_INT32) * samplesPerFrame);
            ALOGV("\tZeroing %d samples per frame at the end of call", samplesPerFrame);
 }

  
 LvmStatus = LVREV_Process(pContext->hInstance,  
                                  pContext->InFrames32,  
                                  pContext->OutFrames32,  
                                  frameCount);  
 }

    LVM_ERROR_CHECK(LvmStatus, "LVREV_Process", "process")
 if(LvmStatus != LVREV_SUCCESS) return -EINVAL;

 if (pContext->auxiliary) {
 for (int i=0; i < frameCount*2; i++) {  
 OutFrames16[i] = clamp16(pContext->OutFrames32[i]>>8);
 }
 } else {
 for (int i=0; i < frameCount*2; i++) {  
 OutFrames16[i] = clamp16((pContext->OutFrames32[i]>>8) + (LVM_INT32)pIn[i]);
 }

 if ((pContext->leftVolume != pContext->prevLeftVolume ||
                pContext->rightVolume != pContext->prevRightVolume) &&
                pContext->volumeMode == REVERB_VOLUME_RAMP) {
            LVM_INT32 vl = (LVM_INT32)pContext->prevLeftVolume << 16;
            LVM_INT32 incl = (((LVM_INT32)pContext->leftVolume << 16) - vl) / frameCount;
            LVM_INT32 vr = (LVM_INT32)pContext->prevRightVolume << 16;
            LVM_INT32 incr = (((LVM_INT32)pContext->rightVolume << 16) - vr) / frameCount;

 for (int i = 0; i < frameCount; i++) {
 OutFrames16[2*i] =
                        clamp16((LVM_INT32)((vl >> 16) * OutFrames16[2*i]) >> 12);
 OutFrames16[2*i+1] =
                        clamp16((LVM_INT32)((vr >> 16) * OutFrames16[2*i+1]) >> 12);

                vl += incl;
                vr += incr;
 }

            pContext->prevLeftVolume = pContext->leftVolume;
            pContext->prevRightVolume = pContext->rightVolume;
 } else if (pContext->volumeMode != REVERB_VOLUME_OFF) {
 if (pContext->leftVolume != REVERB_UNIT_VOLUME ||
                pContext->rightVolume != REVERB_UNIT_VOLUME) {
 for (int i = 0; i < frameCount; i++) {
 OutFrames16[2*i] =
                            clamp16((LVM_INT32)(pContext->leftVolume * OutFrames16[2*i]) >> 12);
 OutFrames16[2*i+1] =
                            clamp16((LVM_INT32)(pContext->rightVolume * OutFrames16[2*i+1]) >> 12);
 }
 }
            pContext->prevLeftVolume = pContext->leftVolume;
            pContext->prevRightVolume = pContext->rightVolume;
            pContext->volumeMode = REVERB_VOLUME_RAMP;
 }
 }

 #ifdef LVM_PCM
    fwrite(OutFrames16, frameCount*sizeof(LVM_INT16)*2, 1, pContext->PcmOutPtr);
    fflush(pContext->PcmOutPtr);
 #endif

 if (pContext->config.outputCfg.accessMode == EFFECT_BUFFER_ACCESS_ACCUMULATE){
 for (int i=0; i<frameCount*2; i++){  
            pOut[i] = clamp16((int32_t)pOut[i] + (int32_t)OutFrames16[i]);
 }
 }else{
        memcpy(pOut, OutFrames16, frameCount*sizeof(LVM_INT16)*2);
 }

 return 0;
}  
