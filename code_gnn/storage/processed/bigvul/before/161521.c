RenderFrameDevToolsAgentHost::~RenderFrameDevToolsAgentHost() {
  RenderFrameDevToolsArray::iterator it =
      std::find(g_agent_host_instances.Get().begin(),
                g_agent_host_instances.Get().end(), this);
  if (it != g_agent_host_instances.Get().end())
    g_agent_host_instances.Get().erase(it);
}
