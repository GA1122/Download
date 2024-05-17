ListValue* GetHostPermissions(const Extension* ext, bool effective_perm) {
  URLPatternSet pattern_set;
  if (effective_perm)
    pattern_set = ext->GetEffectiveHostPermissions();
  else
    pattern_set = ext->GetActivePermissions()->explicit_hosts();

  ListValue* permissions = new ListValue;
  for (URLPatternSet::const_iterator perm = pattern_set.begin();
       perm != pattern_set.end(); ++perm) {
    permissions->Append(new StringValue(perm->GetAsString()));
  }

  return permissions;
}
