void InspectorOverlay::highlightQuad(PassOwnPtr<FloatQuad> quad, const HighlightConfig& highlightConfig)
{
    m_quadHighlightConfig = highlightConfig;
    m_highlightQuad = quad;
    update();
}
