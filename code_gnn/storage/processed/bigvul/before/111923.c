ProfileSyncService::ProfileSyncService(ProfileSyncComponentsFactory* factory,
                                       Profile* profile,
                                       SigninManager* signin_manager,
                                       StartBehavior start_behavior)
    : last_auth_error_(AuthError::None()),
      passphrase_required_reason_(sync_api::REASON_PASSPHRASE_NOT_REQUIRED),
      factory_(factory),
      profile_(profile),
      sync_prefs_(profile_ ? profile_->GetPrefs() : NULL),
      sync_service_url_(kDevServerUrl),
      backend_initialized_(false),
      is_auth_in_progress_(false),
      signin_(signin_manager),
      unrecoverable_error_detected_(false),
      weak_factory_(ALLOW_THIS_IN_INITIALIZER_LIST(this)),
      expect_sync_configuration_aborted_(false),
      clear_server_data_state_(CLEAR_NOT_STARTED),
      encrypted_types_(browser_sync::Cryptographer::SensitiveTypes()),
      encrypt_everything_(false),
      encryption_pending_(false),
      auto_start_enabled_(start_behavior == AUTO_START),
      failed_datatypes_handler_(ALLOW_THIS_IN_INITIALIZER_LIST(this)),
      configure_status_(DataTypeManager::UNKNOWN),
      setup_in_progress_(false) {
#if defined(OS_ANDROID)
  chrome::VersionInfo version_info;
  if (version_info.IsOfficialBuild()) {
    sync_service_url_ = GURL(kSyncServerUrl);
  }
#else
  base::ThreadRestrictions::ScopedAllowIO allow_io;
  chrome::VersionInfo::Channel channel = chrome::VersionInfo::GetChannel();
  if (channel == chrome::VersionInfo::CHANNEL_STABLE ||
      channel == chrome::VersionInfo::CHANNEL_BETA) {
    sync_service_url_ = GURL(kSyncServerUrl);
  }
#endif
}
