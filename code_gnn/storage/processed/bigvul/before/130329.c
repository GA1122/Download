InvalidationData& ensureInvalidationData(Map& map, const typename Map::KeyType& key)
{
    typename Map::AddResult addResult = map.add(key, nullptr);
    if (addResult.isNewEntry)
        addResult.storedValue->value = InvalidationData::create();
    return *addResult.storedValue->value;
}
