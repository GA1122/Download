void ChromotingInstance::HandleExtensionMessage(
    const base::DictionaryValue& data) {
  std::string type;
  std::string message_data;
  if (!data.GetString("type", &type) ||
      !data.GetString("data", &message_data)) {
    LOG(ERROR) << "Invalid extensionMessage.";
    return;
  }
  if (!IsConnected()) {
    return;
  }
  protocol::ExtensionMessage message;
  message.set_type(type);
  message.set_data(message_data);
  client_->host_stub()->DeliverClientMessage(message);
}
