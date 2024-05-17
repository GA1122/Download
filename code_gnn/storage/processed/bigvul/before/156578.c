bool DecodeStorageKey(const std::string& storage_key,
                      std::string* session_tag,
                      int* tab_node_id) {
  base::Pickle pickle(storage_key.c_str(), storage_key.size());
  base::PickleIterator iter(pickle);
  if (!iter.ReadString(session_tag)) {
    return false;
  }
  if (!iter.ReadInt(tab_node_id)) {
    return false;
  }
  return true;
}
