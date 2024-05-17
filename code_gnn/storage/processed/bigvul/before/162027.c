void BrowserChildProcessHostImpl::OnBadMessageReceived(
    const IPC::Message& message) {
  std::string log_message = "Bad message received of type: ";
  if (message.IsValid()) {
    log_message += std::to_string(message.type());
  } else {
    log_message += "unknown";
  }
  TerminateOnBadMessageReceived(log_message);
}
