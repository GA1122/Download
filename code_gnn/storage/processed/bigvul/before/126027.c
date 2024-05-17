void AutomationMouseEventProcessor::OnProcessMouseEventACK(
    bool success,
    const std::string& error_msg) {
  InvokeCallback(automation::Error(error_msg));
}
