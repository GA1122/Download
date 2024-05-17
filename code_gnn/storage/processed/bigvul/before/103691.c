DevToolsAgent::DevToolsAgent(RenderView* render_view)
    : RenderViewObserver(render_view) {
  agent_for_routing_id_[routing_id()] = this;

  CommandLine* cmd = CommandLine::ForCurrentProcess();
  expose_v8_debugger_protocol_ = cmd->HasSwitch(switches::kRemoteShellPort);

  render_view->webview()->setDevToolsAgentClient(this);
  render_view->webview()->devToolsAgent()->setProcessId(
      base::Process::Current().pid());
}
