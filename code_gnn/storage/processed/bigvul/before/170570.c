void EqualizerSetBandLevel(EffectContext *pContext, int band, short Gain){
 int gainRounded;
 if(Gain > 0){
        gainRounded = (int)((Gain+50)/100);
 }else{
        gainRounded = (int)((Gain-50)/100);
 }
    pContext->pBundledContext->bandGaindB[band] = gainRounded;
    pContext->pBundledContext->CurPreset = PRESET_CUSTOM;

 EqualizerUpdateActiveParams(pContext);
 LvmEffect_limitLevel(pContext);
}
