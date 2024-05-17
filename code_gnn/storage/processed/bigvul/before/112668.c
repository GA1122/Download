bool DocumentLoader::isLoading() const
{
#if ENABLE(THREADED_HTML_PARSER)
    if (document() && document()->hasActiveParser())
        return true;
#endif
    return isLoadingMainResource() || !m_subresourceLoaders.isEmpty() || !m_plugInStreamLoaders.isEmpty();
}
