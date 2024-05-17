bool ChromeContentUtilityClient::OnMessageReceived(
    const IPC::Message& message) {
  if (utility_process_running_elevated_)
    return false;

  for (const auto& handler : handlers_) {
    if (handler->OnMessageReceived(message))
      return true;
  }

  return false;
}
