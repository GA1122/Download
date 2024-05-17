void Cache::pruneLiveResources()
{
    if (!m_pruneEnabled)
        return;

    unsigned capacity = liveCapacity();
    if (m_liveSize <= capacity)
        return;

    unsigned targetSize = static_cast<unsigned>(capacity * cTargetPrunePercentage);  
    double currentTime = Frame::currentPaintTimeStamp();
    if (!currentTime)  
        currentTime = WebCore::currentTime();
    
    CachedResource* current = m_liveDecodedResources.m_tail;
    while (current) {
        CachedResource* prev = current->m_prevInLiveResourcesList;
        ASSERT(current->referenced());
        if (current->isLoaded() && current->decodedSize()) {
            double elapsedTime = currentTime - current->m_lastDecodedAccessTime;
            if (elapsedTime < cMinDelayBeforeLiveDecodedPrune)
                return;

            current->destroyDecodedData();

            if (m_liveSize <= targetSize)
                return;
        }
        current = prev;
    }
}
