BrowserContext* ServiceWorkerDevToolsAgentHost::GetBrowserContext() {
  RenderProcessHost* rph = RenderProcessHost::FromID(worker_process_id_);
  return rph ? rph->GetBrowserContext() : nullptr;
}
