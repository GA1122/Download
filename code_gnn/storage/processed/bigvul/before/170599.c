extern "C" int EffectRelease(effect_handle_t handle){
 ReverbContext * pContext = (ReverbContext *)handle;

    ALOGV("\tEffectRelease %p", handle);
 if (pContext == NULL){
        ALOGV("\tLVM_ERROR : EffectRelease called with NULL pointer");
 return -EINVAL;
 }

 #ifdef LVM_PCM
    fclose(pContext->PcmInPtr);
    fclose(pContext->PcmOutPtr);
 #endif
    free(pContext->InFrames32);
    free(pContext->OutFrames32);
 Reverb_free(pContext);
 delete pContext;
 return 0;
}  
