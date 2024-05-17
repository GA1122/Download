std::unique_ptr<Permissions> PackPermissionSet(const PermissionSet& set) {
  std::unique_ptr<Permissions> permissions(new Permissions());

  permissions->permissions.reset(new std::vector<std::string>());
  for (const APIPermission* api : set.apis()) {
    std::unique_ptr<base::Value> value(api->ToValue());
    if (!value) {
      permissions->permissions->push_back(api->name());
    } else {
      std::string name(api->name());
      std::string json;
      base::JSONWriter::Write(*value, &json);
      permissions->permissions->push_back(name + kDelimiter + json);
    }
  }


  permissions->origins.reset(new std::vector<std::string>());
  for (const URLPattern& pattern : set.effective_hosts())
    permissions->origins->push_back(pattern.GetAsString());

  return permissions;
}
