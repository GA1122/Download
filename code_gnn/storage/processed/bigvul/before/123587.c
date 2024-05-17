InspectorController::InspectorController(Page* page, InspectorClient* inspectorClient)
    : m_instrumentingAgents(InstrumentingAgents::create())
    , m_injectedScriptManager(InjectedScriptManager::createForPage())
    , m_state(adoptPtr(new InspectorCompositeState(inspectorClient)))
    , m_overlay(InspectorOverlay::create(page, inspectorClient))
    , m_page(page)
    , m_inspectorClient(inspectorClient)
    , m_agents(m_instrumentingAgents.get(), m_state.get())
    , m_isUnderTest(false)
    , m_deferredAgentsInitialized(false)
{
    InjectedScriptManager* injectedScriptManager = m_injectedScriptManager.get();
    InspectorOverlay* overlay = m_overlay.get();

    m_agents.append(InspectorInspectorAgent::create(m_page, injectedScriptManager));

    OwnPtr<InspectorPageAgent> pageAgentPtr(InspectorPageAgent::create(m_page, injectedScriptManager, inspectorClient, overlay));
    m_pageAgent = pageAgentPtr.get();
    m_agents.append(pageAgentPtr.release());

    OwnPtr<InspectorDOMAgent> domAgentPtr(InspectorDOMAgent::create(m_pageAgent, injectedScriptManager, overlay));
    m_domAgent = domAgentPtr.get();
    m_agents.append(domAgentPtr.release());

    m_agents.append(InspectorDatabaseAgent::create());

    OwnPtr<InspectorTimelineAgent> timelineAgentPtr(InspectorTimelineAgent::create(m_pageAgent, m_domAgent, overlay,
        InspectorTimelineAgent::PageInspector, inspectorClient));
    m_timelineAgent = timelineAgentPtr.get();
    m_agents.append(timelineAgentPtr.release());

    PageScriptDebugServer* pageScriptDebugServer = &PageScriptDebugServer::shared();

    m_agents.append(PageRuntimeAgent::create(injectedScriptManager, pageScriptDebugServer, m_page, m_pageAgent));

    m_agents.append(PageConsoleAgent::create(injectedScriptManager, m_domAgent, m_timelineAgent));

    m_agents.append(InspectorWorkerAgent::create());

    ASSERT_ARG(inspectorClient, inspectorClient);
    m_injectedScriptManager->injectedScriptHost()->init(m_instrumentingAgents.get(), pageScriptDebugServer);
}
