void RenderFrameDevToolsAgentHost::MaybeReattachToRenderFrame() {
  if (!EnsureAgent())
    return;
  for (DevToolsSession* session : sessions())
    session->AttachToAgent(agent_ptr_);
}
