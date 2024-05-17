 std::string MasterPreferences::ExtractPrefString(
    const std::string& name) const {
  std::string result;
  scoped_ptr<base::Value> pref_value;
  if (master_dictionary_->Remove(name, &pref_value)) {
    if (!pref_value->GetAsString(&result))
      NOTREACHED();
  }
  return result;
}
