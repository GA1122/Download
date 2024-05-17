void RenderFrameDevToolsAgentHost::RevokePolicy() {
  if (!frame_host_)
    return;

  bool process_has_agents = false;
  RenderProcessHost* process_host = frame_host_->GetProcess();
  for (RenderFrameDevToolsAgentHost* agent : g_agent_host_instances.Get()) {
    if (!agent->IsAttached())
      continue;
    if (agent->frame_host_ && agent->frame_host_ != frame_host_ &&
        agent->frame_host_->GetProcess() == process_host) {
      process_has_agents = true;
    }
  }

  if (!process_has_agents) {
    if (base::FeatureList::IsEnabled(features::kNetworkService))
      GetNetworkService()->SetRawHeadersAccess(process_host->GetID(), false);
    ChildProcessSecurityPolicyImpl::GetInstance()->RevokeReadRawCookies(
        process_host->GetID());
  }
}
