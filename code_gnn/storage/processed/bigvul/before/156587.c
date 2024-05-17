std::string SessionStore::GetHeaderStorageKey(const std::string& session_tag) {
  return EncodeStorageKey(session_tag, TabNodePool::kInvalidTabNodeID);
}
