void InspectorResourceAgent::setExtraHTTPHeaders(ErrorString*, const RefPtr<JSONObject>& headers)
{
    m_state->setObject(ResourceAgentState::extraRequestHeaders, headers);
}
