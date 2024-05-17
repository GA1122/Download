inline CachedShapingResults* HarfBuzzRunCache::find(const std::wstring& key) const
{
    CachedShapingResultsMap::const_iterator it = m_harfBuzzRunMap.find(key);

    return it != m_harfBuzzRunMap.end() ? it->second : 0;
}
