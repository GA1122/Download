void LayerTreeCoordinator::purgeBackingStores()
{
    HashSet<WebCore::CoordinatedGraphicsLayer*>::iterator end = m_registeredLayers.end();
    for (HashSet<WebCore::CoordinatedGraphicsLayer*>::iterator it = m_registeredLayers.begin(); it != end; ++it)
        (*it)->purgeBackingStores();

    ASSERT(!m_directlyCompositedImageRefCounts.size());
    m_updateAtlases.clear();
}
