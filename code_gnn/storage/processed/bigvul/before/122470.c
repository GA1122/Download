void InspectorController::didRemovePageOverlay(const GraphicsLayer* layer)
{
    if (m_layerTreeAgent)
        m_layerTreeAgent->didRemovePageOverlay(layer);
}
