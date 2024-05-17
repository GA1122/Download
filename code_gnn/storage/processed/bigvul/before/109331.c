 void InspectorPageAgent::setShowScrollBottleneckRects(ErrorString*, bool show)
{
    m_state->setBoolean(PageAgentState::pageAgentShowScrollBottleneckRects, show);
    m_client->setShowScrollBottleneckRects(show);
}
