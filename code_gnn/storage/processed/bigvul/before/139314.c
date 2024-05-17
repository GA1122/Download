bool HarfBuzzRunCache::insert(const std::wstring& key, CachedShapingResults* data)
{
    std::pair<CachedShapingResultsMap::iterator, bool> results =
        m_harfBuzzRunMap.insert(CachedShapingResultsMap::value_type(key, data));

    if (!results.second)
        return false;

    CachedShapingResultsLRUNode* node = new CachedShapingResultsLRUNode(results.first);

    m_harfBuzzRunLRU.push_back(node);
    data->lru = --m_harfBuzzRunLRU.end();

    if (m_harfBuzzRunMap.size() > cHarfBuzzCacheMaxSize) {
        CachedShapingResultsLRUNode* lru = m_harfBuzzRunLRU.front();
        CachedShapingResults* foo = lru->entry->second;
        m_harfBuzzRunMap.erase(lru->entry);
        m_harfBuzzRunLRU.pop_front();
        delete foo;
        delete lru;
    }

    return true;
}
