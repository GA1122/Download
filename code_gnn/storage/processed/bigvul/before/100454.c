bool WebRuntimeFeatures::isDatabaseEnabled()
{
#if ENABLE(DATABASE)
    return RuntimeEnabledFeatures::databaseEnabled();
#else
    return false;
#endif
}
