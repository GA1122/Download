InspectorPageAgent::InspectorPageAgent(InstrumentingAgents* instrumentingAgents, Page* page, InspectorCompositeState* inspectorState, InjectedScriptManager* injectedScriptManager, InspectorClient* client, InspectorOverlay* overlay)
    : InspectorBaseAgent<InspectorPageAgent>("Page", instrumentingAgents, inspectorState)
    , m_page(page)
    , m_injectedScriptManager(injectedScriptManager)
    , m_client(client)
    , m_frontend(0)
    , m_overlay(overlay)
    , m_lastScriptIdentifier(0)
    , m_enabled(false)
    , m_geolocationOverridden(false)
    , m_ignoreScriptsEnabledNotification(false)
    , m_didForceCompositingMode(false)
{
}
