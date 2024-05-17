const ValidityMessage& ValidityMessages::GetMessageOrDefault(
    ServerFieldType field) const {
  MessageMap::const_iterator iter = messages_.find(field);
  return iter != messages_.end() ? iter->second : default_message_;
}
