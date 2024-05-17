void FrameLoader::handledOnloadEvents()
{
    m_client->dispatchDidHandleOnloadEvents();

#if ENABLE(OFFLINE_WEB_APPLICATIONS)
    if (documentLoader())
        documentLoader()->applicationCacheHost()->stopDeferringEvents();
#endif
}
