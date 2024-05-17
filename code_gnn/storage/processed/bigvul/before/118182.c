void ValidityMessages::Set(
    ServerFieldType field, const ValidityMessage& message) {
  MessageMap::iterator iter = messages_.find(field);
  if (iter != messages_.end()) {
    if (!iter->second.text.empty())
      return;

    messages_.erase(iter);
  }

  messages_.insert(MessageMap::value_type(field, message));
}
