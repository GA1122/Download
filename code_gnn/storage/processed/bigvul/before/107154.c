LayerTreeHostQt::~LayerTreeHostQt()
{
    HashSet<WebCore::WebGraphicsLayer*> registeredLayers;
    registeredLayers.swap(m_registeredLayers);

    HashSet<WebCore::WebGraphicsLayer*>::iterator end = registeredLayers.end();
    for (HashSet<WebCore::WebGraphicsLayer*>::iterator it = registeredLayers.begin(); it != end; ++it)
        (*it)->setWebGraphicsLayerClient(0);
}
