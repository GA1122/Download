void ExtensionSystemImpl::Shared::InitPrefs() {
  bool extensions_disabled =
      profile_->GetPrefs()->GetBoolean(prefs::kDisableExtensions) ||
      CommandLine::ForCurrentProcess()->HasSwitch(switches::kDisableExtensions);
  extension_prefs_ = ExtensionPrefs::Create(
      profile_->GetPrefs(),
      profile_->GetPath().AppendASCII(ExtensionService::kInstallDirectoryName),
      ExtensionPrefValueMapFactory::GetForProfile(profile_),
      extensions_disabled);
  lazy_background_task_queue_.reset(new LazyBackgroundTaskQueue(profile_));
  event_router_.reset(new EventRouter(profile_, extension_prefs_.get()));

  state_store_.reset(new StateStore(
      profile_,
      profile_->GetPath().AppendASCII(ExtensionService::kStateStoreName),
      true));
  rules_store_.reset(new StateStore(
      profile_,
      profile_->GetPath().AppendASCII(ExtensionService::kRulesStoreName),
      false));

  shell_window_geometry_cache_.reset(new ShellWindowGeometryCache(
      profile_, extension_prefs_.get()));

  blacklist_.reset(new Blacklist(extension_prefs_.get()));

  standard_management_policy_provider_.reset(
      new StandardManagementPolicyProvider(extension_prefs_.get()));
}
