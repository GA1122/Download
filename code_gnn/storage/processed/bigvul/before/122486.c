void InspectorController::willAddPageOverlay(const GraphicsLayer* layer)
{
    if (m_layerTreeAgent)
        m_layerTreeAgent->willAddPageOverlay(layer);
}
