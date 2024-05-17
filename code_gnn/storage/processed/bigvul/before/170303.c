UsbChooserContext::GetAllGrantedObjects() {
  std::vector<std::unique_ptr<ChooserContextBase::Object>> objects =
      ChooserContextBase::GetAllGrantedObjects();

  for (const auto& map_entry : ephemeral_devices_) {
    const GURL& requesting_origin = map_entry.first.first;
    const GURL& embedding_origin = map_entry.first.second;

    if (!CanRequestObjectPermission(requesting_origin, embedding_origin))
      continue;

    for (const std::string& guid : map_entry.second) {
      auto dict_it = ephemeral_dicts_.find(guid);
      DCHECK(dict_it != ephemeral_dicts_.end());
      auto object = dict_it->second.CreateDeepCopy();
      objects.push_back(std::make_unique<ChooserContextBase::Object>(
          requesting_origin, embedding_origin, object.get(), "preference",
          is_incognito_));
    }
  }

  return objects;
}
