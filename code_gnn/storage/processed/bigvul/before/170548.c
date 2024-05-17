void LE_reset(LoudnessEnhancerContext *pContext)
{
    ALOGV("  > LE_reset(%p)", pContext);

 if (pContext->mCompressor != NULL) {
 float targetAmp = pow(10, pContext->mTargetGainmB/2000.0f);  
        ALOGV("LE_reset(): Target gain=%dmB <=> factor=%.2fX", pContext->mTargetGainmB, targetAmp);
        pContext->mCompressor->Initialize(targetAmp, pContext->mConfig.inputCfg.samplingRate);
 } else {
        ALOGE("LE_reset(%p): null compressors, can't apply target gain", pContext);
 }
}
