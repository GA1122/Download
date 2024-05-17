void DisplayItemList::updateValidlyCachedClientsIfNeeded() const
{
    if (!m_validlyCachedClientsDirty)
        return;

    m_validlyCachedClients.clear();
    m_validlyCachedClientsDirty = false;

    DisplayItemClient lastAddedClient = nullptr;
    for (const DisplayItem& displayItem : m_currentDisplayItems) {
        if (displayItem.client() == lastAddedClient)
            continue;
        if (displayItem.isCacheable()) {
            lastAddedClient = displayItem.client();
            m_validlyCachedClients.add(lastAddedClient);
        }
    }
}
