void WebRuntimeFeatures::enablePrefixedEncryptedMedia(bool enable)
{
    RuntimeEnabledFeatures::setPrefixedEncryptedMediaEnabled(enable);
    RuntimeEnabledFeatures::setEncryptedMediaAnyVersionEnabled(
        RuntimeEnabledFeatures::encryptedMediaEnabled()
        || RuntimeEnabledFeatures::prefixedEncryptedMediaEnabled());
}
