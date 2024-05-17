const std::string AutoFillManager::IDToGUID(int id) {
  if (id == 0)
    return std::string();

  std::map<int, std::string>::const_iterator iter = id_guid_map_.find(id);
  if (iter == id_guid_map_.end()) {
    NOTREACHED();
    return std::string();
  }

  return iter->second;
}
