void AudioFlinger::EffectHandle::setControl(bool hasControl, bool signal, bool enabled)
{
    ALOGV("setControl %p control %d", this, hasControl);

    mHasControl = hasControl;
    mEnabled = enabled;

 if (signal && mEffectClient != 0) {
        mEffectClient->controlStatusChanged(hasControl);
 }
}
