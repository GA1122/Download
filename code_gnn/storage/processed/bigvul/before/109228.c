void InspectorOverlay::drawQuadHighlight()
{
    if (!m_highlightQuad)
        return;

    Highlight highlight;
    buildQuadHighlight(m_page, *m_highlightQuad, m_quadHighlightConfig, &highlight);
    evaluateInOverlay("drawQuadHighlight", buildObjectForHighlight(highlight));
}
