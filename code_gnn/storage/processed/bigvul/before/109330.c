void InspectorPageAgent::setShowPaintRects(ErrorString*, bool show)
{
    m_state->setBoolean(PageAgentState::pageAgentShowPaintRects, show);
    m_client->setShowPaintRects(show);

    if (!show && mainFrame() && mainFrame()->view())
        mainFrame()->view()->invalidate();
}
