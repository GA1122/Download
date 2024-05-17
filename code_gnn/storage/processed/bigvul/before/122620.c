void Extension::RuntimeData::ClearTabSpecificPermissions(int tab_id) {
  CHECK_GE(tab_id, 0);
  tab_specific_permissions_.erase(tab_id);
}
