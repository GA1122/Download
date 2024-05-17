ExtensionService* TestExtensionSystem::CreateExtensionService(
    const CommandLine* command_line,
    const FilePath& install_directory,
    bool autoupdate_enabled) {
  bool extensions_disabled =
      command_line && command_line->HasSwitch(switches::kDisableExtensions);


  extension_prefs_ = ExtensionPrefs::Create(
      profile_->GetPrefs(),
      install_directory,
      ExtensionPrefValueMapFactory::GetForProfile(profile_),
      extensions_disabled);
  state_store_.reset(new StateStore(profile_, new TestingValueStore()));
  shell_window_geometry_cache_.reset(
      new ShellWindowGeometryCache(profile_, extension_prefs_.get()));
  blacklist_.reset(new Blacklist(extension_prefs_.get()));
  standard_management_policy_provider_.reset(
      new StandardManagementPolicyProvider(extension_prefs_.get()));
  management_policy_.reset(new ManagementPolicy());
  management_policy_->RegisterProvider(
      standard_management_policy_provider_.get());
  extension_service_.reset(new ExtensionService(profile_,
                                                command_line,
                                                install_directory,
                                                extension_prefs_.get(),
                                                blacklist_.get(),
                                                autoupdate_enabled,
                                                true));
  extension_service_->ClearProvidersForTesting();
  return extension_service_.get();
}
