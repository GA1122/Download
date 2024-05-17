void ExtensionSystemImpl::Shared::Init(bool extensions_enabled) {
  const CommandLine* command_line = CommandLine::ForCurrentProcess();

  message_service_.reset(new MessageService(lazy_background_task_queue_.get()));
  navigation_observer_.reset(new NavigationObserver(profile_));

  bool allow_noisy_errors = !command_line->HasSwitch(switches::kNoErrorDialogs);
  ExtensionErrorReporter::Init(allow_noisy_errors);

  user_script_master_ = new UserScriptMaster(profile_);

  bool autoupdate_enabled = true;
#if defined(OS_CHROMEOS)
  if (!extensions_enabled)
    autoupdate_enabled = false;
  else
    autoupdate_enabled = !command_line->HasSwitch(switches::kGuestSession);
#endif
  extension_service_.reset(new ExtensionService(
      profile_,
      CommandLine::ForCurrentProcess(),
      profile_->GetPath().AppendASCII(ExtensionService::kInstallDirectoryName),
      extension_prefs_.get(),
      blacklist_.get(),
      autoupdate_enabled,
      extensions_enabled));

  {
    management_policy_.reset(new ManagementPolicy);
    RegisterManagementPolicyProviders();
  }

  bool skip_session_extensions = false;
#if defined(OS_CHROMEOS)
  skip_session_extensions = !chromeos::UserManager::Get()->IsUserLoggedIn();
#endif
  extension_service_->component_loader()->AddDefaultComponentExtensions(
      skip_session_extensions);
  if (command_line->HasSwitch(switches::kLoadComponentExtension)) {
    CommandLine::StringType path_list = command_line->GetSwitchValueNative(
        switches::kLoadComponentExtension);
    StringTokenizerT<CommandLine::StringType,
        CommandLine::StringType::const_iterator> t(path_list,
                                                   FILE_PATH_LITERAL(","));
    while (t.GetNext()) {
      base::ThreadRestrictions::ScopedAllowIO allow_io;
      extension_service_->component_loader()->AddOrReplace(
          FilePath(t.token()));
    }
  }
  extension_service_->Init();

  if (extensions_enabled) {
    if (command_line->HasSwitch(switches::kLoadExtension)) {
      CommandLine::StringType path_list = command_line->GetSwitchValueNative(
          switches::kLoadExtension);
      StringTokenizerT<CommandLine::StringType,
          CommandLine::StringType::const_iterator> t(path_list,
                                                     FILE_PATH_LITERAL(","));
      while (t.GetNext()) {
        UnpackedInstaller::Create(extension_service_.get())->
            LoadFromCommandLine(FilePath(t.token()));
      }
    }
  }

  ChromeURLDataManager::AddDataSource(profile_,
      new ExtensionIconSource(profile_));

  CHECK(!ProfileManager::IsImportProcess(*command_line));
  if (g_browser_process->profile_manager()->will_import()) {
    extension_service_->InitEventRoutersAfterImport();
  } else {
    extension_service_->InitEventRouters();
  }

  extension_warning_service_.reset(new ExtensionWarningService(profile_));
  extension_warning_badge_service_.reset(
      new ExtensionWarningBadgeService(profile_));
  extension_warning_service_->AddObserver(
      extension_warning_badge_service_.get());
}
