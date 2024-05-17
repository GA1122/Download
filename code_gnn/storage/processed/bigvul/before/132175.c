void RenderFrameImpl::AddMessageToConsole(ConsoleMessageLevel level,
                                          const std::string& message) {
  if (devtools_agent_)
    devtools_agent_->AddMessageToConsole(level, message);
}
