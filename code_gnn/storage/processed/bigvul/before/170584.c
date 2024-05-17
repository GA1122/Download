audio_devices_t VirtualizerGetVirtualizationMode(EffectContext *pContext) {
 audio_devices_t virtDevice = AUDIO_DEVICE_NONE;
 if ((pContext->pBundledContext->bVirtualizerEnabled == LVM_TRUE)
 && (pContext->pBundledContext->bVirtualizerTempDisabled == LVM_FALSE)) {
 if (pContext->pBundledContext->nVirtualizerForcedDevice != AUDIO_DEVICE_NONE) {
            virtDevice = pContext->pBundledContext->nVirtualizerForcedDevice;
 } else {
            virtDevice = pContext->pBundledContext->nOutputDevice;
 }
 }
    ALOGV("VirtualizerGetVirtualizationMode() returning 0x%x", virtDevice);
 return virtDevice;
}
