void BlinkTestRunner::ShowDevTools(const std::string& settings,
                                   const std::string& frontend_url) {
  Send(new ShellViewHostMsg_ShowDevTools(
      routing_id(), settings, frontend_url));
}
