void RenderProcessHostImpl::Release(int routing_id) {
  DCHECK(render_widget_hosts_.Lookup(routing_id) != NULL);
  render_widget_hosts_.Remove(routing_id);

  CancelResourceRequests(routing_id);

#if defined(OS_WIN)
  const CommandLine& browser_command_line =
      *CommandLine::ForCurrentProcess();
  if (browser_command_line.HasSwitch(switches::kAuditHandles) ||
      browser_command_line.HasSwitch(switches::kAuditAllHandles)) {
    DumpHandles();

    return;
  }
#endif
  if (!run_renderer_in_process())
    Cleanup();
}
