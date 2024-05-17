scoped_ptr<Permissions> PackPermissionSet(const PermissionSet* set) {
  Permissions* permissions(new Permissions());

  permissions->permissions.reset(new std::vector<std::string>());
  for (APIPermissionSet::const_iterator i = set->apis().begin();
       i != set->apis().end(); ++i) {
    scoped_ptr<base::Value> value(i->ToValue());
    if (!value) {
      permissions->permissions->push_back(i->name());
    } else {
      std::string name(i->name());
      std::string json;
      base::JSONWriter::Write(value.get(), &json);
      permissions->permissions->push_back(name + kDelimiter + json);
    }
  }

  permissions->origins.reset(new std::vector<std::string>());
  URLPatternSet hosts = set->explicit_hosts();
  for (URLPatternSet::const_iterator i = hosts.begin(); i != hosts.end(); ++i)
    permissions->origins->push_back(i->GetAsString());

  return scoped_ptr<Permissions>(permissions);
 }
