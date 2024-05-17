inline void HarfBuzzRunCache::remove(CachedShapingResults* node)
{
    CachedShapingResultsLRUNode* lruNode = *node->lru;

    m_harfBuzzRunLRU.erase(node->lru);
    m_harfBuzzRunMap.erase(lruNode->entry);
    delete lruNode;
    delete node;
}
