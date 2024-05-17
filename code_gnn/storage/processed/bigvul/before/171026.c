void AudioFlinger::EffectHandle::setEnabled(bool enabled)
{
 if (mEffectClient != 0) {
        mEffectClient->enableStatusChanged(enabled);
 }
}
