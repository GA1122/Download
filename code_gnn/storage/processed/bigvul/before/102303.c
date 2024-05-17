ExtensionPermissionSet* ExtensionPrefs::ReadExtensionPrefPermissionSet(
    const std::string& extension_id,
    const std::string& pref_key) {
  if (!GetExtensionPref(extension_id))
    return NULL;

  ExtensionAPIPermissionSet apis;
  const ListValue* api_values = NULL;
  std::string api_pref = JoinPrefs(pref_key, kPrefAPIs);
  if (ReadExtensionPrefList(extension_id, api_pref, &api_values)) {
    ExtensionPermissionsInfo* info = ExtensionPermissionsInfo::GetInstance();
    for (size_t i = 0; i < api_values->GetSize(); ++i) {
      std::string permission_name;
      if (api_values->GetString(i, &permission_name)) {
        ExtensionAPIPermission *permission = info->GetByName(permission_name);
        if (permission)
          apis.insert(permission->id());
      }
    }
  }

  URLPatternSet explicit_hosts;
  ReadExtensionPrefURLPatternSet(
      extension_id, JoinPrefs(pref_key, kPrefExplicitHosts),
      &explicit_hosts, Extension::kValidHostPermissionSchemes);

  URLPatternSet scriptable_hosts;
  ReadExtensionPrefURLPatternSet(
      extension_id, JoinPrefs(pref_key, kPrefScriptableHosts),
      &scriptable_hosts, UserScript::kValidUserScriptSchemes);

  return new ExtensionPermissionSet(apis, explicit_hosts, scriptable_hosts);
}
