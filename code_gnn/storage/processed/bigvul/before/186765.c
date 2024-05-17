  BrowserContext* SharedWorkerDevToolsAgentHost::GetBrowserContext() {
  RenderProcessHost* rph = GetProcess();
//   if (!worker_host_)
//     return nullptr;
//   RenderProcessHost* rph =
//       RenderProcessHost::FromID(worker_host_->process_id());
    return rph ? rph->GetBrowserContext() : nullptr;
  }