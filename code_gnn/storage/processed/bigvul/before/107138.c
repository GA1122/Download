void LayerTreeHostQt::purgeBackingStores()
{
    HashSet<WebCore::WebGraphicsLayer*>::iterator end = m_registeredLayers.end();
    for (HashSet<WebCore::WebGraphicsLayer*>::iterator it = m_registeredLayers.begin(); it != end; ++it)
        (*it)->purgeBackingStores();

    ASSERT(!m_directlyCompositedImageRefCounts.size());
}
