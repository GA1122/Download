Response InspectorNetworkAgent::setBlockedURLs(
    std::unique_ptr<protocol::Array<String>> urls) {
  std::unique_ptr<protocol::DictionaryValue> new_list =
      protocol::DictionaryValue::create();
  for (size_t i = 0; i < urls->length(); i++)
    new_list->setBoolean(urls->get(i), true);
  state_->setObject(NetworkAgentState::kBlockedURLs, std::move(new_list));
  return Response::OK();
}
