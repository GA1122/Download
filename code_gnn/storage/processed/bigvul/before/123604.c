PassOwnPtr<InspectorPageAgent> InspectorPageAgent::create(Page* page, InjectedScriptManager* injectedScriptManager, InspectorClient* client, InspectorOverlay* overlay)
{
    return adoptPtr(new InspectorPageAgent(page, injectedScriptManager, client, overlay));
}
