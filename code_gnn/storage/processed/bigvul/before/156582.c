std::string EncodeStorageKey(const std::string& session_tag, int tab_node_id) {
  base::Pickle pickle;
  pickle.WriteString(session_tag);
  pickle.WriteInt(tab_node_id);
  return std::string(static_cast<const char*>(pickle.data()), pickle.size());
}
