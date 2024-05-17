void WebDevToolsAgentImpl::didComposite()
{
    TRACE_EVENT_END1(TRACE_DISABLED_BY_DEFAULT("devtools.timeline"), "CompositeLayers", "mainFrame", mainFrame());
    if (InspectorController* ic = inspectorController())
        ic->didComposite();
}
