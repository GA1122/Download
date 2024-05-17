HarfBuzzRunCache::~HarfBuzzRunCache()
{
    for (CachedShapingResultsMap::iterator it = m_harfBuzzRunMap.begin(); it != m_harfBuzzRunMap.end(); ++it)
        delete it->second;
    for (CachedShapingResultsLRU::iterator it = m_harfBuzzRunLRU.begin(); it != m_harfBuzzRunLRU.end(); ++it)
        delete *it;
}
