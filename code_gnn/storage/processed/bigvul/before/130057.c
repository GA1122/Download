void OverrideUIString(uint32_t hash, const base::string16& string) {
  int resource_id = GetResourceIndex(hash);
  if (resource_id == -1)
    return;

  ui::ResourceBundle::GetSharedInstance().OverrideLocaleStringResource(
      resource_id, string);
}
