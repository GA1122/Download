String Resource::GetMemoryDumpName() const {
  return String::Format(
      "web_cache/%s_resources/%ld",
      ResourceTypeToString(GetType(), Options().initiator_info.name),
      identifier_);
}
