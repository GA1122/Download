bool DisplayItemList::clientCacheIsValid(DisplayItemClient client) const
{
    if (skippingCache())
        return false;
    updateValidlyCachedClientsIfNeeded();
    return m_validlyCachedClients.contains(client);
}
