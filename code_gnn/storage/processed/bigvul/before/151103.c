void DevToolsWindow::RegisterProfilePrefs(
    user_prefs::PrefRegistrySyncable* registry) {
  registry->RegisterDictionaryPref(prefs::kDevToolsEditedFiles);
  registry->RegisterDictionaryPref(prefs::kDevToolsFileSystemPaths);
  registry->RegisterStringPref(prefs::kDevToolsAdbKey, std::string());

  registry->RegisterBooleanPref(prefs::kDevToolsDiscoverUsbDevicesEnabled,
                                true);
  registry->RegisterBooleanPref(prefs::kDevToolsPortForwardingEnabled, false);
  registry->RegisterBooleanPref(prefs::kDevToolsPortForwardingDefaultSet,
                                false);
  registry->RegisterDictionaryPref(prefs::kDevToolsPortForwardingConfig);
  registry->RegisterBooleanPref(prefs::kDevToolsDiscoverTCPTargetsEnabled,
                                true);
  registry->RegisterListPref(prefs::kDevToolsTCPDiscoveryConfig);
  registry->RegisterDictionaryPref(prefs::kDevToolsPreferences);
}
