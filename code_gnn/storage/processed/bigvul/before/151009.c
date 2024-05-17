void DevToolsUIBindings::JsonReceived(const DispatchCallback& callback,
                                      int result,
                                      const std::string& message) {
  if (result != net::OK) {
    callback.Run(nullptr);
    return;
  }
  base::StringValue message_value(message);
  callback.Run(&message_value);
}
