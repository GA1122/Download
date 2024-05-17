void EqualizerLimitBandLevels(EffectContext *pContext) {
 LVM_ControlParams_t ActiveParams;  
    LVM_ReturnStatus_en     LvmStatus=LVM_SUCCESS;  

  
 LvmStatus = LVM_GetControlParameters(pContext->pBundledContext->hInstance, &ActiveParams);
    LVM_ERROR_CHECK(LvmStatus, "LVM_GetControlParameters", "EqualizerLimitBandLevels")

 int maxGain = 0;
 int avgGain = 0;
 int avgCount = 0;
 for (int i = 0; i < FIVEBAND_NUMBANDS; i++) {
 if (pContext->pBundledContext->bandGaindB[i] >= maxGain) {
 int tmpMaxGain = pContext->pBundledContext->bandGaindB[i];
 int tmpAvgGain = 0;
 int tmpAvgCount = 0;
 for (int j = 0; j < FIVEBAND_NUMBANDS; j++) {
 int gain = pContext->pBundledContext->bandGaindB[j];
 if (j == i || gain < 0)
 continue;
 if (gain > tmpMaxGain) {
                    tmpAvgGain = -1;
 break;
 }

 int weight = 1;
 if (j < (i + 2) && j > (i - 2))
                    weight = 4;
                tmpAvgGain += weight * gain;
                tmpAvgCount += weight;
 }
 if (tmpAvgGain >= avgGain) {
                maxGain = tmpMaxGain;
                avgGain = tmpAvgGain;
                avgCount = tmpAvgCount;
 }
 }
 ActiveParams.pEQNB_BandDefinition[i].Frequency = EQNB_5BandPresetsFrequencies[i];
 ActiveParams.pEQNB_BandDefinition[i].QFactor = EQNB_5BandPresetsQFactors[i];
 ActiveParams.pEQNB_BandDefinition[i].Gain = pContext->pBundledContext->bandGaindB[i];
 }

 int gainCorrection = 0;
 if (maxGain + pContext->pBundledContext->volume > 0) {
        gainCorrection = maxGain + pContext->pBundledContext->volume;
 }
 if (avgCount) {
        gainCorrection += avgGain/avgCount;
 }

    ALOGV("EqualizerLimitBandLevels() gainCorrection %d maxGain %d avgGain %d avgCount %d",
            gainCorrection, maxGain, avgGain, avgCount);

 ActiveParams.VC_EffectLevel  = pContext->pBundledContext->volume - gainCorrection;
 if (ActiveParams.VC_EffectLevel < -96) {
 ActiveParams.VC_EffectLevel = -96;
 }

  
 LvmStatus = LVM_SetControlParameters(pContext->pBundledContext->hInstance, &ActiveParams);
    LVM_ERROR_CHECK(LvmStatus, "LVM_SetControlParameters", "EqualizerLimitBandLevels")

 if(pContext->pBundledContext->firstVolume == LVM_TRUE){
 LvmStatus = LVM_SetVolumeNoSmoothing(pContext->pBundledContext->hInstance, &ActiveParams);
        LVM_ERROR_CHECK(LvmStatus, "LVM_SetVolumeNoSmoothing", "LvmBundle_process")
        ALOGV("\tLVM_VOLUME: Disabling Smoothing for first volume change to remove spikes/clicks");
        pContext->pBundledContext->firstVolume = LVM_FALSE;
 }
}
