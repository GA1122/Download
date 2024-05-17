PassOwnPtr<InspectorPageAgent> InspectorPageAgent::create(InstrumentingAgents* instrumentingAgents, Page* page, InspectorCompositeState* state, InjectedScriptManager* injectedScriptManager, InspectorClient* client, InspectorOverlay* overlay)
{
    return adoptPtr(new InspectorPageAgent(instrumentingAgents, page, state, injectedScriptManager, client, overlay));
}
