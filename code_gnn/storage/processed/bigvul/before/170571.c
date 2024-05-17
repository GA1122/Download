void EqualizerSetPreset(EffectContext *pContext, int preset){

    pContext->pBundledContext->CurPreset = preset;

 for (int i=0; i<FIVEBAND_NUMBANDS; i++)
 {
        pContext->pBundledContext->bandGaindB[i] =
                EQNB_5BandSoftPresets[i + preset * FIVEBAND_NUMBANDS];
 }

 EqualizerUpdateActiveParams(pContext);
 LvmEffect_limitLevel(pContext);

 return;
}
