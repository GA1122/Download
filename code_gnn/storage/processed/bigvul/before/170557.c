extern "C" int EffectRelease(effect_handle_t handle){
    ALOGV("\n\tEffectRelease start %p", handle);
 EffectContext * pContext = (EffectContext *)handle;

    ALOGV("\tEffectRelease start handle: %p, context %p", handle, pContext->pBundledContext);
 if (pContext == NULL){
        ALOGV("\tLVM_ERROR : EffectRelease called with NULL pointer");
 return -EINVAL;
 }

 SessionContext *pSessionContext = &GlobalSessionMemory[pContext->pBundledContext->SessionNo];

 if(pContext->EffectType == LVM_BASS_BOOST) {
        ALOGV("\tEffectRelease LVM_BASS_BOOST Clearing global intstantiated flag");
        pSessionContext->bBassInstantiated = LVM_FALSE;
 if(pContext->pBundledContext->SamplesToExitCountBb > 0){
            pContext->pBundledContext->NumberEffectsEnabled--;
 }
        pContext->pBundledContext->SamplesToExitCountBb = 0;
 } else if(pContext->EffectType == LVM_VIRTUALIZER) {
        ALOGV("\tEffectRelease LVM_VIRTUALIZER Clearing global intstantiated flag");
        pSessionContext->bVirtualizerInstantiated = LVM_FALSE;
 if(pContext->pBundledContext->SamplesToExitCountVirt > 0){
            pContext->pBundledContext->NumberEffectsEnabled--;
 }
        pContext->pBundledContext->SamplesToExitCountVirt = 0;
 } else if(pContext->EffectType == LVM_EQUALIZER) {
        ALOGV("\tEffectRelease LVM_EQUALIZER Clearing global intstantiated flag");
        pSessionContext->bEqualizerInstantiated =LVM_FALSE;
 if(pContext->pBundledContext->SamplesToExitCountEq > 0){
            pContext->pBundledContext->NumberEffectsEnabled--;
 }
        pContext->pBundledContext->SamplesToExitCountEq = 0;
 } else if(pContext->EffectType == LVM_VOLUME) {
        ALOGV("\tEffectRelease LVM_VOLUME Clearing global intstantiated flag");
        pSessionContext->bVolumeInstantiated = LVM_FALSE;
 if (pContext->pBundledContext->bVolumeEnabled == LVM_TRUE){
            pContext->pBundledContext->NumberEffectsEnabled--;
 }
 } else {
        ALOGV("\tLVM_ERROR : EffectRelease : Unsupported effect\n\n\n\n\n\n\n");
 }

 Effect_setEnabled(pContext, LVM_FALSE);

 if ((pSessionContext->bBassInstantiated == LVM_FALSE) &&
 (pSessionContext->bVolumeInstantiated == LVM_FALSE) &&
 (pSessionContext->bEqualizerInstantiated ==LVM_FALSE) &&
 (pSessionContext->bVirtualizerInstantiated==LVM_FALSE))
 {
 #ifdef LVM_PCM
 if (pContext->pBundledContext->PcmInPtr != NULL) {
            fclose(pContext->pBundledContext->PcmInPtr);
            pContext->pBundledContext->PcmInPtr = NULL;
 }
 if (pContext->pBundledContext->PcmOutPtr != NULL) {
            fclose(pContext->pBundledContext->PcmOutPtr);
            pContext->pBundledContext->PcmOutPtr = NULL;
 }
 #endif


 for(int i=0; i<LVM_MAX_SESSIONS; i++){
 if(SessionIndex[i] == pContext->pBundledContext->SessionId){
 SessionIndex[i] = LVM_UNUSED_SESSION;
                ALOGV("\tEffectRelease: Clearing SessionIndex SessionNo %d for SessionId %d\n",
                        i, pContext->pBundledContext->SessionId);
 break;
 }
 }

        ALOGV("\tEffectRelease: All effects are no longer instantiated\n");
        pSessionContext->bBundledEffectsEnabled = LVM_FALSE;
        pSessionContext->pBundledContext = LVM_NULL;
        ALOGV("\tEffectRelease: Freeing LVM Bundle memory\n");
 LvmEffect_free(pContext);
        ALOGV("\tEffectRelease: Deleting LVM Bundle context %p\n", pContext->pBundledContext);
 if (pContext->pBundledContext->workBuffer != NULL) {
            free(pContext->pBundledContext->workBuffer);
 }
 delete pContext->pBundledContext;
        pContext->pBundledContext = LVM_NULL;
 }
 delete pContext;

    ALOGV("\tEffectRelease end\n");
 return 0;

}  
