base::ProcessHandle BrowserRenderProcessHost::ExecuteRenderer(
    CommandLine* cmd_line,
    bool has_cmd_prefix) {
  return sandbox::StartProcess(cmd_line);
}
