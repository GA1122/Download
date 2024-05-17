void WebRuntimeFeatures::enableLocalStorage(bool enable)
{
#if ENABLE(DOM_STORAGE)
    RuntimeEnabledFeatures::setLocalStorageEnabled(enable);
#endif
}
