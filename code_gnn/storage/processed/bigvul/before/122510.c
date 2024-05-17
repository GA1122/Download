 void InspectorResourceAgent::setCacheDisabled(ErrorString*, bool cacheDisabled)
 {
     m_state->setBoolean(ResourceAgentState::cacheDisabled, cacheDisabled);
    if (cacheDisabled)
        memoryCache()->evictResources();
    for (LocalFrame* frame = m_pageAgent->mainFrame(); frame; frame = frame->tree().traverseNext())
        frame->document()->fetcher()->garbageCollectDocumentResources();
}
