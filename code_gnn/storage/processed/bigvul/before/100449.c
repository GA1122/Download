void WebRuntimeFeatures::enableDatabase(bool enable)
{
#if ENABLE(DATABASE)
    RuntimeEnabledFeatures::setDatabaseEnabled(enable);
#endif
}
