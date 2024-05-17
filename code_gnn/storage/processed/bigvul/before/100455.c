bool WebRuntimeFeatures::isLocalStorageEnabled()
{
#if ENABLE(DOM_STORAGE)
    return RuntimeEnabledFeatures::localStorageEnabled();
#else
    return false;
#endif
}
