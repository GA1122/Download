int AutoFillManager::GUIDToID(const std::string& guid) {
  static int last_id = 1;

  if (!guid::IsValidGUID(guid))
    return 0;

  std::map<std::string, int>::const_iterator iter = guid_id_map_.find(guid);
  if (iter == guid_id_map_.end()) {
    guid_id_map_[guid] = last_id;
    id_guid_map_[last_id] = guid;
    return last_id++;
  } else {
    return iter->second;
  }
}
