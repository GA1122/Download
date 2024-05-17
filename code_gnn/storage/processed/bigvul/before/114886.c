ListValue* GetAPIPermissions(const Extension* ext) {
  ListValue* permissions = new ListValue;
  std::set<std::string> perm_list =
      ext->GetActivePermissions()->GetAPIsAsStrings();
  for (std::set<std::string>::const_iterator perm = perm_list.begin();
       perm != perm_list.end(); ++perm) {
    permissions->Append(new StringValue(perm->c_str()));
  }
  return permissions;
}
