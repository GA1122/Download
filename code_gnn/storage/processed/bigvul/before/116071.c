void ExtensionSettingsHandler::HandleToggleDeveloperMode(
      const ListValue* args) {
  Profile* profile = Profile::FromWebUI(web_ui());
  bool developer_mode =
      profile->GetPrefs()->GetBoolean(prefs::kExtensionsUIDeveloperMode);
  profile->GetPrefs()->SetBoolean(
      prefs::kExtensionsUIDeveloperMode, !developer_mode);
  HandleRequestExtensionsData(NULL);
}
