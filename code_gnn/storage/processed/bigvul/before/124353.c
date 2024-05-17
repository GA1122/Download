void MessageService::PostMessageFromNativeProcess(int port_id,
                                                  const std::string& message) {
  PostMessage(port_id, message);
}
