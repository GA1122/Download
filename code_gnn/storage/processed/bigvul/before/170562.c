int Effect_setEnabled(EffectContext *pContext, bool enabled)
{
    ALOGV("\tEffect_setEnabled() type %d, enabled %d", pContext->EffectType, enabled);

 if (enabled) {
 bool tempDisabled = false;
 switch (pContext->EffectType) {
 case LVM_BASS_BOOST:
 if (pContext->pBundledContext->bBassEnabled == LVM_TRUE) {
                     ALOGV("\tEffect_setEnabled() LVM_BASS_BOOST is already enabled");
 return -EINVAL;
 }
 if(pContext->pBundledContext->SamplesToExitCountBb <= 0){
                    pContext->pBundledContext->NumberEffectsEnabled++;
 }
                pContext->pBundledContext->SamplesToExitCountBb =
 (LVM_INT32)(pContext->pBundledContext->SamplesPerSecond*0.1);
                pContext->pBundledContext->bBassEnabled = LVM_TRUE;
                tempDisabled = pContext->pBundledContext->bBassTempDisabled;
 break;
 case LVM_EQUALIZER:
 if (pContext->pBundledContext->bEqualizerEnabled == LVM_TRUE) {
                    ALOGV("\tEffect_setEnabled() LVM_EQUALIZER is already enabled");
 return -EINVAL;
 }
 if(pContext->pBundledContext->SamplesToExitCountEq <= 0){
                    pContext->pBundledContext->NumberEffectsEnabled++;
 }
                pContext->pBundledContext->SamplesToExitCountEq =
 (LVM_INT32)(pContext->pBundledContext->SamplesPerSecond*0.1);
                pContext->pBundledContext->bEqualizerEnabled = LVM_TRUE;
 break;
 case LVM_VIRTUALIZER:
 if (pContext->pBundledContext->bVirtualizerEnabled == LVM_TRUE) {
                    ALOGV("\tEffect_setEnabled() LVM_VIRTUALIZER is already enabled");
 return -EINVAL;
 }
 if(pContext->pBundledContext->SamplesToExitCountVirt <= 0){
                    pContext->pBundledContext->NumberEffectsEnabled++;
 }
                pContext->pBundledContext->SamplesToExitCountVirt =
 (LVM_INT32)(pContext->pBundledContext->SamplesPerSecond*0.1);
                pContext->pBundledContext->bVirtualizerEnabled = LVM_TRUE;
                tempDisabled = pContext->pBundledContext->bVirtualizerTempDisabled;
 break;
 case LVM_VOLUME:
 if (pContext->pBundledContext->bVolumeEnabled == LVM_TRUE) {
                    ALOGV("\tEffect_setEnabled() LVM_VOLUME is already enabled");
 return -EINVAL;
 }
                pContext->pBundledContext->NumberEffectsEnabled++;
                pContext->pBundledContext->bVolumeEnabled = LVM_TRUE;
 break;
 default:
                ALOGV("\tEffect_setEnabled() invalid effect type");
 return -EINVAL;
 }
 if (!tempDisabled) {
 LvmEffect_enable(pContext);
 }
 } else {
 switch (pContext->EffectType) {
 case LVM_BASS_BOOST:
 if (pContext->pBundledContext->bBassEnabled == LVM_FALSE) {
                    ALOGV("\tEffect_setEnabled() LVM_BASS_BOOST is already disabled");
 return -EINVAL;
 }
                pContext->pBundledContext->bBassEnabled = LVM_FALSE;
 break;
 case LVM_EQUALIZER:
 if (pContext->pBundledContext->bEqualizerEnabled == LVM_FALSE) {
                    ALOGV("\tEffect_setEnabled() LVM_EQUALIZER is already disabled");
 return -EINVAL;
 }
                pContext->pBundledContext->bEqualizerEnabled = LVM_FALSE;
 break;
 case LVM_VIRTUALIZER:
 if (pContext->pBundledContext->bVirtualizerEnabled == LVM_FALSE) {
                    ALOGV("\tEffect_setEnabled() LVM_VIRTUALIZER is already disabled");
 return -EINVAL;
 }
                pContext->pBundledContext->bVirtualizerEnabled = LVM_FALSE;
 break;
 case LVM_VOLUME:
 if (pContext->pBundledContext->bVolumeEnabled == LVM_FALSE) {
                    ALOGV("\tEffect_setEnabled() LVM_VOLUME is already disabled");
 return -EINVAL;
 }
                pContext->pBundledContext->bVolumeEnabled = LVM_FALSE;
 break;
 default:
                ALOGV("\tEffect_setEnabled() invalid effect type");
 return -EINVAL;
 }
 LvmEffect_disable(pContext);
 }

 return 0;
}
