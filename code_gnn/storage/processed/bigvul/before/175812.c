void LvmEffect_limitLevel(EffectContext *pContext) {
 LVM_ControlParams_t ActiveParams;  
    LVM_ReturnStatus_en     LvmStatus=LVM_SUCCESS;  

  
 LvmStatus = LVM_GetControlParameters(pContext->pBundledContext->hInstance, &ActiveParams);
    LVM_ERROR_CHECK(LvmStatus, "LVM_GetControlParameters", "LvmEffect_limitLevel")

 int gainCorrection = 0;
 float energyContribution = 0;
 float energyCross = 0;
 float energyBassBoost = 0;
 float crossCorrection = 0;

 if (pContext->pBundledContext->bEqualizerEnabled == LVM_TRUE) {
 for (int i = 0; i < FIVEBAND_NUMBANDS; i++) {
 float bandFactor = pContext->pBundledContext->bandGaindB[i]/15.0;
 float bandCoefficient = LimitLevel_bandEnergyCoefficient[i];
 float bandEnergy = bandFactor * bandCoefficient * bandCoefficient;
 if (bandEnergy > 0)
                energyContribution += bandEnergy;
 }

 float bandFactorSum = 0;
 for (int i = 0; i < FIVEBAND_NUMBANDS-1; i++) {
 float bandFactor1 = pContext->pBundledContext->bandGaindB[i]/15.0;
 float bandFactor2 = pContext->pBundledContext->bandGaindB[i+1]/15.0;

 if (bandFactor1 > 0 && bandFactor2 > 0) {
 float crossEnergy = bandFactor1 * bandFactor2 *
 LimitLevel_bandEnergyCrossCoefficient[i];
                bandFactorSum += bandFactor1 * bandFactor2;

 if (crossEnergy > 0)
                    energyCross += crossEnergy;
 }
 }
        bandFactorSum -= 1.0;
 if (bandFactorSum > 0)
            crossCorrection = bandFactorSum * 0.7;
 }

 if (pContext->pBundledContext->bBassEnabled == LVM_TRUE) {
 float boostFactor = (pContext->pBundledContext->BassStrengthSaved)/1000.0;
 float boostCoefficient = LimitLevel_bassBoostEnergyCoefficient;

        energyContribution += boostFactor * boostCoefficient * boostCoefficient;

 for (int i = 0; i < FIVEBAND_NUMBANDS; i++) {
 float bandFactor = pContext->pBundledContext->bandGaindB[i]/15.0;
 float bandCrossCoefficient = LimitLevel_bassBoostEnergyCrossCoefficient[i];
 float bandEnergy = boostFactor * bandFactor *
                    bandCrossCoefficient;
 if (bandEnergy > 0)
                energyBassBoost += bandEnergy;
 }
 }

 if (pContext->pBundledContext->bVirtualizerEnabled == LVM_TRUE) {
        energyContribution += LimitLevel_virtualizerContribution *
 LimitLevel_virtualizerContribution;
 }

 double totalEnergyEstimation = sqrt(energyContribution + energyCross + energyBassBoost) -
            crossCorrection;
    ALOGV(" TOTAL energy estimation: %0.2f", totalEnergyEstimation);

 int maxLevelRound = (int)(totalEnergyEstimation + 0.99);
 if (maxLevelRound + pContext->pBundledContext->volume > 0) {
        gainCorrection = maxLevelRound + pContext->pBundledContext->volume;
 }

 ActiveParams.VC_EffectLevel  = pContext->pBundledContext->volume - gainCorrection;
 if (ActiveParams.VC_EffectLevel < -96) {
 ActiveParams.VC_EffectLevel = -96;
 }
    ALOGV("\tVol:%d, GainCorrection: %d, Actual vol: %d", pContext->pBundledContext->volume,
            gainCorrection, ActiveParams.VC_EffectLevel);

  
 LvmStatus = LVM_SetControlParameters(pContext->pBundledContext->hInstance, &ActiveParams);
    LVM_ERROR_CHECK(LvmStatus, "LVM_SetControlParameters", "LvmEffect_limitLevel")

 if (pContext->pBundledContext->firstVolume == LVM_TRUE){
 LvmStatus = LVM_SetVolumeNoSmoothing(pContext->pBundledContext->hInstance, &ActiveParams);
        LVM_ERROR_CHECK(LvmStatus, "LVM_SetVolumeNoSmoothing", "LvmBundle_process")
        ALOGV("\tLVM_VOLUME: Disabling Smoothing for first volume change to remove spikes/clicks");
        pContext->pBundledContext->firstVolume = LVM_FALSE;
 }
}
