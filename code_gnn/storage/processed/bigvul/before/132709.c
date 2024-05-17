void ChromotingInstance::DeliverHostMessage(
    const protocol::ExtensionMessage& message) {
  scoped_ptr<base::DictionaryValue> data(new base::DictionaryValue());
  data->SetString("type", message.type());
  data->SetString("data", message.data());
  PostLegacyJsonMessage("extensionMessage", data.Pass());
}
