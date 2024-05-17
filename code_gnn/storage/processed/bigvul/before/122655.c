    Extension::RuntimeData::GetTabSpecificPermissions(int tab_id) const {
  CHECK_GE(tab_id, 0);
  TabPermissionsMap::const_iterator it = tab_specific_permissions_.find(tab_id);
  return (it != tab_specific_permissions_.end()) ? it->second : NULL;
}
