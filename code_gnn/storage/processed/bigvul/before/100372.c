base::ProcessHandle BrowserRenderProcessHost::GetRendererProcessHandle() {
  if (run_renderer_in_process())
    return base::Process::Current().handle();
  return process_.handle();
}
