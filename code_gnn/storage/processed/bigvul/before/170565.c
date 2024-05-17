int32_t EqualizerGetBandLevel(EffectContext *pContext, int32_t band){
 return pContext->pBundledContext->bandGaindB[band] * 100;
}
