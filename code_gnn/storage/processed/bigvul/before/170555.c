extern "C" int EffectCreate(const effect_uuid_t *uuid,
 int32_t             sessionId,
 int32_t             ioId __unused,
 effect_handle_t *pHandle){
 int ret = 0;
 int sessionNo;
 int i;
 EffectContext *pContext = NULL;
 bool newBundle = false;
 SessionContext *pSessionContext;

    ALOGV("\n\tEffectCreate start session %d", sessionId);

 if (pHandle == NULL || uuid == NULL){
        ALOGV("\tLVM_ERROR : EffectCreate() called with NULL pointer");
        ret = -EINVAL;
 goto exit;
 }

 if(LvmInitFlag == LVM_FALSE){
 LvmInitFlag = LVM_TRUE;
        ALOGV("\tEffectCreate - Initializing all global memory");
 LvmGlobalBundle_init();
 }

 for(i=0; i<LVM_MAX_SESSIONS; i++){
 if((SessionIndex[i] == LVM_UNUSED_SESSION)||(SessionIndex[i] == sessionId)){
            sessionNo       = i;
 SessionIndex[i] = sessionId;
            ALOGV("\tEffectCreate: Allocating SessionNo %d for SessionId %d\n", sessionNo,sessionId);
 break;
 }
 }

 if(i==LVM_MAX_SESSIONS){
        ALOGV("\tLVM_ERROR : Cannot find memory to allocate for current session");
        ret = -EINVAL;
 goto exit;
 }

    pContext = new EffectContext;

 if(GlobalSessionMemory[sessionNo].bBundledEffectsEnabled == LVM_FALSE){
        ALOGV("\tEffectCreate - This is the first effect in current sessionId %d sessionNo %d",
                sessionId, sessionNo);

 GlobalSessionMemory[sessionNo].bBundledEffectsEnabled = LVM_TRUE;
 GlobalSessionMemory[sessionNo].pBundledContext        = new BundledEffectContext;
        newBundle = true;

        pContext->pBundledContext = GlobalSessionMemory[sessionNo].pBundledContext;
        pContext->pBundledContext->SessionNo = sessionNo;
        pContext->pBundledContext->SessionId = sessionId;
        pContext->pBundledContext->hInstance                = NULL;
        pContext->pBundledContext->bVolumeEnabled           = LVM_FALSE;
        pContext->pBundledContext->bEqualizerEnabled        = LVM_FALSE;
        pContext->pBundledContext->bBassEnabled             = LVM_FALSE;
        pContext->pBundledContext->bBassTempDisabled        = LVM_FALSE;
        pContext->pBundledContext->bVirtualizerEnabled      = LVM_FALSE;
        pContext->pBundledContext->bVirtualizerTempDisabled = LVM_FALSE;
        pContext->pBundledContext->nOutputDevice            = AUDIO_DEVICE_NONE;
        pContext->pBundledContext->nVirtualizerForcedDevice = AUDIO_DEVICE_NONE;
        pContext->pBundledContext->NumberEffectsEnabled = 0;
        pContext->pBundledContext->NumberEffectsCalled = 0;
        pContext->pBundledContext->firstVolume              = LVM_TRUE;
        pContext->pBundledContext->volume                   = 0;

 #ifdef LVM_PCM
 char fileName[256];
        snprintf(fileName, 256, "/data/tmp/bundle_%p_pcm_in.pcm", pContext->pBundledContext);
        pContext->pBundledContext->PcmInPtr = fopen(fileName, "w");
 if (pContext->pBundledContext->PcmInPtr == NULL) {
            ALOGV("cannot open %s", fileName);
            ret = -EINVAL;
 goto exit;
 }

        snprintf(fileName, 256, "/data/tmp/bundle_%p_pcm_out.pcm", pContext->pBundledContext);
        pContext->pBundledContext->PcmOutPtr = fopen(fileName, "w");
 if (pContext->pBundledContext->PcmOutPtr == NULL) {
            ALOGV("cannot open %s", fileName);
            fclose(pContext->pBundledContext->PcmInPtr);
           pContext->pBundledContext->PcmInPtr = NULL;
           ret = -EINVAL;
 goto exit;
 }
 #endif

  
        pContext->pBundledContext->BassStrengthSaved = 0;
        pContext->pBundledContext->VirtStrengthSaved = 0;
        pContext->pBundledContext->CurPreset = PRESET_CUSTOM;
        pContext->pBundledContext->levelSaved               = 0;
        pContext->pBundledContext->bMuteEnabled             = LVM_FALSE;
        pContext->pBundledContext->bStereoPositionEnabled   = LVM_FALSE;
        pContext->pBundledContext->positionSaved            = 0;
        pContext->pBundledContext->workBuffer               = NULL;
        pContext->pBundledContext->frameCount               = -1;
        pContext->pBundledContext->SamplesToExitCountVirt = 0;
        pContext->pBundledContext->SamplesToExitCountBb = 0;
        pContext->pBundledContext->SamplesToExitCountEq = 0;

 for (int i = 0; i < FIVEBAND_NUMBANDS; i++) {
            pContext->pBundledContext->bandGaindB[i] = EQNB_5BandSoftPresets[i];
 }

        ALOGV("\tEffectCreate - Calling LvmBundle_init");
        ret = LvmBundle_init(pContext);

 if (ret < 0){
            ALOGV("\tLVM_ERROR : EffectCreate() Bundle init failed");
 goto exit;
 }
 }
 else{
        ALOGV("\tEffectCreate - Assigning memory for previously created effect on sessionNo %d",
                sessionNo);
        pContext->pBundledContext =
 GlobalSessionMemory[sessionNo].pBundledContext;
 }
    ALOGV("\tEffectCreate - pBundledContext is %p", pContext->pBundledContext);

    pSessionContext = &GlobalSessionMemory[pContext->pBundledContext->SessionNo];

 if (memcmp(uuid, &gBassBoostDescriptor.uuid, sizeof(effect_uuid_t)) == 0){
        ALOGV("\tEffectCreate - Effect to be created is LVM_BASS_BOOST");
        pSessionContext->bBassInstantiated = LVM_TRUE;
        pContext->pBundledContext->SamplesToExitCountBb = 0;

        pContext->itfe       = &gLvmEffectInterface;
        pContext->EffectType = LVM_BASS_BOOST;
 } else if (memcmp(uuid, &gVirtualizerDescriptor.uuid, sizeof(effect_uuid_t)) == 0){
        ALOGV("\tEffectCreate - Effect to be created is LVM_VIRTUALIZER");
        pSessionContext->bVirtualizerInstantiated=LVM_TRUE;
        pContext->pBundledContext->SamplesToExitCountVirt = 0;

        pContext->itfe       = &gLvmEffectInterface;
        pContext->EffectType = LVM_VIRTUALIZER;
 } else if (memcmp(uuid, &gEqualizerDescriptor.uuid, sizeof(effect_uuid_t)) == 0){
        ALOGV("\tEffectCreate - Effect to be created is LVM_EQUALIZER");
        pSessionContext->bEqualizerInstantiated = LVM_TRUE;
        pContext->pBundledContext->SamplesToExitCountEq = 0;

        pContext->itfe       = &gLvmEffectInterface;
        pContext->EffectType = LVM_EQUALIZER;
 } else if (memcmp(uuid, &gVolumeDescriptor.uuid, sizeof(effect_uuid_t)) == 0){
        ALOGV("\tEffectCreate - Effect to be created is LVM_VOLUME");
        pSessionContext->bVolumeInstantiated = LVM_TRUE;

        pContext->itfe       = &gLvmEffectInterface;
        pContext->EffectType = LVM_VOLUME;
 }
 else{
        ALOGV("\tLVM_ERROR : EffectCreate() invalid UUID");
        ret = -EINVAL;
 goto exit;
 }

exit:
 if (ret != 0) {
 if (pContext != NULL) {
 if (newBundle) {
 GlobalSessionMemory[sessionNo].bBundledEffectsEnabled = LVM_FALSE;
 SessionIndex[sessionNo] = LVM_UNUSED_SESSION;
 delete pContext->pBundledContext;
 }
 delete pContext;
 }
 *pHandle = (effect_handle_t)NULL;
 } else {
 *pHandle = (effect_handle_t)pContext;
 }
    ALOGV("\tEffectCreate end..\n\n");
 return ret;
}  
