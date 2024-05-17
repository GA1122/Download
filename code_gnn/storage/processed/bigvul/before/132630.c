void BlinkTestRunner::CloseDevTools() {
  Send(new ShellViewHostMsg_CloseDevTools(routing_id()));
  WebDevToolsAgent* agent =
      render_view()->GetMainRenderFrame()->GetWebFrame()->devToolsAgent();
  if (agent)
    agent->detach();
}
