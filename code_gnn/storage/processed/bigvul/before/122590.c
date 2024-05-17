void WebDevToolsAgentImpl::willComposite()
{
    TRACE_EVENT_BEGIN1(TRACE_DISABLED_BY_DEFAULT("devtools.timeline"), "CompositeLayers", "mainFrame", mainFrame());
    if (InspectorController* ic = inspectorController())
        ic->willComposite();
}
