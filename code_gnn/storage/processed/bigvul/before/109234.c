void InspectorOverlay::getHighlight(Highlight* highlight) const
{
    if (!m_highlightNode && !m_highlightQuad)
        return;

    highlight->type = HighlightTypeRects;
    if (m_highlightNode)
        buildNodeHighlight(m_highlightNode.get(), m_nodeHighlightConfig, highlight);
    else
        buildQuadHighlight(m_page, *m_highlightQuad, m_quadHighlightConfig, highlight);
}
