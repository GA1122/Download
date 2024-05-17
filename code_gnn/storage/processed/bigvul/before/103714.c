DevToolsAgentFilter::DevToolsAgentFilter()
    : message_handled_(false),
      render_thread_loop_(MessageLoop::current()) {
  WebDevToolsAgent::setMessageLoopDispatchHandler(
      &DevToolsAgentFilter::DispatchMessageLoop);
}
