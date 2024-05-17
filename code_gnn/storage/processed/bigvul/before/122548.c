void WebDevToolsAgentImpl::didCreateScriptContext(WebLocalFrameImpl* webframe, int worldId)
{
    if (worldId)
        return;
    if (WebCore::LocalFrame* frame = webframe->frame())
        frame->script().setContextDebugId(m_hostId);
}
