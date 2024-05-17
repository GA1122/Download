void WebRuntimeFeatures::enableSessionStorage(bool enable)
{
#if ENABLE(DOM_STORAGE)
    RuntimeEnabledFeatures::setSessionStorageEnabled(enable);
#endif
}
