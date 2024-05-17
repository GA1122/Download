bool DOMMessageQueue::WaitForMessage(std::string* message) {
  DCHECK(message);
  if (!renderer_crashed_ && message_queue_.empty()) {
    message_loop_runner_ =
        new MessageLoopRunner(MessageLoopRunner::QuitMode::IMMEDIATE);
    message_loop_runner_->Run();
  }
  return PopMessage(message);
}
