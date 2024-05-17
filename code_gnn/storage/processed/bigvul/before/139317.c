inline void HarfBuzzRunCache::moveToBack(CachedShapingResults* node)
{
    CachedShapingResultsLRUNode* lruNode = *node->lru;
    m_harfBuzzRunLRU.erase(node->lru);
    m_harfBuzzRunLRU.push_back(lruNode);
    node->lru = --m_harfBuzzRunLRU.end();
}
