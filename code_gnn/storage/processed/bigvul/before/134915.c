void ChromeBrowserMainPartsChromeos::PreProfileInit() {

  BootTimesRecorder::Get()->RecordChromeMainStats();
  LoginEventRecorder::Get()->SetDelegate(BootTimesRecorder::Get());

  DeviceSettingsService::Get()->Load();


  g_browser_process->platform_part()->InitializeChromeUserManager();
  g_browser_process->platform_part()->InitializeSessionManager();

  ScreenLocker::InitClass();

  g_browser_process->profile_manager();

  input_method::Initialize();

  ProfileHelper::Get()->Initialize();

  bool immediate_login = parsed_command_line().HasSwitch(switches::kLoginUser);
  if (immediate_login) {
    logging::RedirectChromeLogging(parsed_command_line());

    app_order_loader_.reset(
        new default_app_order::ExternalLoader(false  ));
  }

  if (!app_order_loader_) {
    app_order_loader_.reset(
        new default_app_order::ExternalLoader(true  ));
  }

  media::SoundsManager::Create();

  NoteTakingHelper::Initialize();

  AccessibilityManager::Initialize();

  if (chromeos::GetAshConfig() != ash::Config::MASH) {
    MagnificationManager::Initialize();
  }

  base::PostTaskWithTraitsAndReplyWithResult(
      FROM_HERE, {base::MayBlock(), base::TaskPriority::BACKGROUND},
      base::Bind(&version_loader::GetVersion, version_loader::VERSION_FULL),
      base::Bind(&ChromeOSVersionCallback));

  if (parsed_command_line().HasSwitch(::switches::kTestType) ||
      ShouldAutoLaunchKioskApp(parsed_command_line())) {
    WizardController::SetZeroDelays();
  }

  arc_kiosk_app_manager_.reset(new ArcKioskAppManager());

  if (!base::CommandLine::ForCurrentProcess()->HasSwitch(
          ::switches::kDisableZeroBrowsersOpenForTests)) {
    g_browser_process->platform_part()->RegisterKeepAlive();
  }

  chromeos::AccelerometerReader::GetInstance()->Initialize(
      base::CreateSequencedTaskRunnerWithTraits(
          {base::MayBlock(), base::TaskPriority::BACKGROUND,
           base::TaskShutdownBehavior::SKIP_ON_SHUTDOWN}));

  ChromeBrowserMainPartsLinux::PreProfileInit();

  keyboard::InitializeKeyboardResources();

  if (lock_screen_apps::StateController::IsEnabled()) {
    lock_screen_apps_state_controller_ =
        std::make_unique<lock_screen_apps::StateController>();
    lock_screen_apps_state_controller_->Initialize();
  }

  if (immediate_login) {
    const std::string cryptohome_id =
        parsed_command_line().GetSwitchValueASCII(switches::kLoginUser);
    const AccountId account_id(
        cryptohome::Identification::FromString(cryptohome_id).GetAccountId());

    user_manager::UserManager* user_manager = user_manager::UserManager::Get();

    if (policy::IsDeviceLocalAccountUser(account_id.GetUserEmail(), nullptr) &&
        !user_manager->IsKnownUser(account_id)) {
      chrome::AttemptUserExit();
      return;
    }

    std::string user_id_hash =
        parsed_command_line().GetSwitchValueASCII(switches::kLoginProfile);
    session_manager::SessionManager::Get()->CreateSessionForRestart(
        account_id, user_id_hash);
    VLOG(1) << "Relaunching browser for user: " << account_id.Serialize()
            << " with hash: " << user_id_hash;
  }
}
