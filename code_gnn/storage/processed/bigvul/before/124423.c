void WebRuntimeFeatures::enableEncryptedMedia(bool enable)
{
    RuntimeEnabledFeatures::setEncryptedMediaEnabled(enable);
    RuntimeEnabledFeatures::setEncryptedMediaAnyVersionEnabled(
        RuntimeEnabledFeatures::encryptedMediaEnabled()
        || RuntimeEnabledFeatures::prefixedEncryptedMediaEnabled());
}
