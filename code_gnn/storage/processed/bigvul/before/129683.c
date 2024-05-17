void ResourceFetcher::clearPreloads()
{
#if PRELOAD_DEBUG
    printPreloadStats();
#endif
    if (!m_preloads)
        return;

    ListHashSet<Resource*>::iterator end = m_preloads->end();
    for (ListHashSet<Resource*>::iterator it = m_preloads->begin(); it != end; ++it) {
        Resource* res = *it;
        res->decreasePreloadCount();
        bool deleted = res->deleteIfPossible();
        if (!deleted && res->preloadResult() == Resource::PreloadNotReferenced)
            memoryCache()->remove(res);
    }
    m_preloads.clear();
}
