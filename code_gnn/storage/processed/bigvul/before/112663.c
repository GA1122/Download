void DocumentLoader::handledOnloadEvents()
{
    m_wasOnloadHandled = true;
    applicationCacheHost()->stopDeferringEvents();
}
