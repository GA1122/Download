void TestingAutomationProvider::GetMultiProfileInfo(
    base::DictionaryValue* args, IPC::Message* reply_message) {
  scoped_ptr<DictionaryValue> return_value(new DictionaryValue);
  ProfileManager* profile_manager = g_browser_process->profile_manager();
  const ProfileInfoCache& profile_info_cache =
      profile_manager->GetProfileInfoCache();
  return_value->SetBoolean("enabled",
      profile_manager->IsMultipleProfilesEnabled());

  ListValue* profiles = new ListValue;
  for (size_t index = 0; index < profile_info_cache.GetNumberOfProfiles();
       ++index) {
    DictionaryValue* item = new DictionaryValue;
    item->SetString("name", profile_info_cache.GetNameOfProfileAtIndex(index));
    item->SetString("path",
                    profile_info_cache.GetPathOfProfileAtIndex(index).value());
    profiles->Append(item);
  }
  return_value->Set("profiles", profiles);
  AutomationJSONReply(this, reply_message).SendSuccess(return_value.get());
}
