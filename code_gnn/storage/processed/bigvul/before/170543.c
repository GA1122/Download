int LELib_Release(effect_handle_t handle) {
 LoudnessEnhancerContext * pContext = (LoudnessEnhancerContext *)handle;

    ALOGV("LELib_Release %p", handle);
 if (pContext == NULL) {
 return -EINVAL;
 }
    pContext->mState = LOUDNESS_ENHANCER_STATE_UNINITIALIZED;
 if (pContext->mCompressor != NULL) {
 delete pContext->mCompressor;
        pContext->mCompressor = NULL;
 }
 delete pContext;

 return 0;
}
