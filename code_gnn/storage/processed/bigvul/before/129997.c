int ChromeBrowserMainParts::PreMainMessageLoopRunImpl() {
  TRACE_EVENT0("startup", "ChromeBrowserMainParts::PreMainMessageLoopRunImpl");
  TRACK_SCOPED_REGION(
      "Startup", "ChromeBrowserMainParts::PreMainMessageLoopRunImpl");

  SCOPED_UMA_HISTOGRAM_LONG_TIMER("Startup.PreMainMessageLoopRunImplLongTime");
  const base::TimeTicks start_time_step1 = base::TimeTicks::Now();

#if defined(OS_WIN)
  IncognitoModePrefs::InitializePlatformParentalControls();
#endif

#if defined(ENABLE_EXTENSIONS)
  if (!variations::GetVariationParamValue(
      "LightSpeed", "EarlyInitStartup").empty()) {
    BrowserThread::PostTask(
        BrowserThread::FILE_USER_BLOCKING,
        FROM_HERE,
        base::Bind(
            base::IgnoreResult(&extensions::FeatureProvider::GetAPIFeatures)));
  }
#endif

#if !defined(OS_ANDROID)
  StartMetricsRecording();
#endif   

  if (!base::debug::BeingDebugged()) {
    browser_process_->watchdog_thread();
  }

  browser_process_->PreMainMessageLoopRun();

  browser_shutdown::ReadLastShutdownInfo();

#if defined(OS_WIN)
  bool already_running = browser_util::IsBrowserAlreadyRunning();

  if (parsed_command_line().HasSwitch(switches::kUninstall)) {
    return DoUninstallTasks(already_running);
  }

  if (parsed_command_line().HasSwitch(switches::kHideIcons) ||
      parsed_command_line().HasSwitch(switches::kShowIcons)) {
    return ChromeBrowserMainPartsWin::HandleIconsCommands(
        parsed_command_line_);
  }

  ui::SelectFileDialog::SetFactory(new ChromeSelectFileDialogFactory(
      BrowserThread::GetMessageLoopProxyForThread(BrowserThread::IO)));
#endif   

  if (parsed_command_line().HasSwitch(switches::kMakeDefaultBrowser)) {
    bool is_managed = g_browser_process->local_state()->IsManagedPreference(
        prefs::kDefaultBrowserSettingEnabled);
    if (is_managed && !g_browser_process->local_state()->GetBoolean(
        prefs::kDefaultBrowserSettingEnabled)) {
      return static_cast<int>(chrome::RESULT_CODE_ACTION_DISALLOWED_BY_POLICY);
    }

    return ShellIntegration::SetAsDefaultBrowser() ?
        static_cast<int>(content::RESULT_CODE_NORMAL_EXIT) :
        static_cast<int>(chrome::RESULT_CODE_SHELL_INTEGRATION_FAILED);
  }

#if defined(USE_AURA)
  CHECK(aura::Env::GetInstance());
#endif   

#if !defined(OS_ANDROID)
  if (parsed_command_line().HasSwitch(switches::kPackExtension)) {
    extensions::StartupHelper extension_startup_helper;
    if (extension_startup_helper.PackExtension(parsed_command_line()))
      return content::RESULT_CODE_NORMAL_EXIT;
    return chrome::RESULT_CODE_PACK_EXTENSION_ERROR;
  }

  notify_result_ = process_singleton_->NotifyOtherProcessOrCreate();
  switch (notify_result_) {
    case ProcessSingleton::PROCESS_NONE:
      break;

    case ProcessSingleton::PROCESS_NOTIFIED:
#if defined(OS_POSIX) && !defined(OS_MACOSX)
      printf("%s\n", base::SysWideToNativeMB(base::UTF16ToWide(
          l10n_util::GetStringUTF16(IDS_USED_EXISTING_BROWSER))).c_str());
#endif   

      if (parsed_command_line().HasSwitch(switches::kTestType))
        return chrome::RESULT_CODE_NORMAL_EXIT_PROCESS_NOTIFIED;
      return content::RESULT_CODE_NORMAL_EXIT;

    case ProcessSingleton::PROFILE_IN_USE:
      return chrome::RESULT_CODE_PROFILE_IN_USE;

    case ProcessSingleton::LOCK_ERROR:
      LOG(ERROR) << "Failed to create a ProcessSingleton for your profile "
                    "directory. This means that running multiple instances "
                    "would start multiple browser processes rather than "
                    "opening a new window in the existing process. Aborting "
                    "now to avoid profile corruption.";
      return chrome::RESULT_CODE_PROFILE_IN_USE;

    default:
      NOTREACHED();
  }
#endif   


  std::string try_chrome =
      parsed_command_line().GetSwitchValueASCII(switches::kTryChromeAgain);
  if (!try_chrome.empty()) {
#if defined(OS_WIN)
    int try_chrome_int;
    base::StringToInt(try_chrome, &try_chrome_int);
    TryChromeDialogView::Result answer = TryChromeDialogView::Show(
        try_chrome_int,
        base::Bind(&ChromeProcessSingleton::SetActiveModalDialog,
                   base::Unretained(process_singleton_.get())));
    if (answer == TryChromeDialogView::NOT_NOW)
      return chrome::RESULT_CODE_NORMAL_EXIT_CANCEL;
    if (answer == TryChromeDialogView::UNINSTALL_CHROME)
      return chrome::RESULT_CODE_NORMAL_EXIT_EXP2;
    if (answer == TryChromeDialogView::TRY_CHROME_AS_DEFAULT) {
      if (ShellIntegration::CanSetAsDefaultBrowser() ==
          ShellIntegration::SET_DEFAULT_UNATTENDED)
        ShellIntegration::SetAsDefaultBrowser();
    }
#else
    return content::RESULT_CODE_NORMAL_EXIT;
#endif   
  }

#if defined(OS_WIN)
  if (!already_running && upgrade_util::DoUpgradeTasks(parsed_command_line()))
    return content::RESULT_CODE_NORMAL_EXIT;

  if (ChromeBrowserMainPartsWin::CheckMachineLevelInstall())
    return chrome::RESULT_CODE_MACHINE_LEVEL_INSTALL_EXISTS;
#endif   

#if defined(OS_LINUX) && !defined(OS_CHROMEOS)
  if (sxs_linux::ShouldMigrateUserDataDir())
    return sxs_linux::MigrateUserDataDir();
#endif   

  PreProfileInit();


  metrics::MetricsService::SetExecutionPhase(
      metrics::MetricsService::CREATE_PROFILE,
      g_browser_process->local_state());

  UMA_HISTOGRAM_TIMES("Startup.PreMainMessageLoopRunImplStep1Time",
                      base::TimeTicks::Now() - start_time_step1);

  profile_ = CreatePrimaryProfile(parameters(),
                                  user_data_dir_,
                                  parsed_command_line());
  if (!profile_)
    return content::RESULT_CODE_NORMAL_EXIT;

#if !defined(OS_ANDROID)
  const base::TimeTicks start_time_step2 = base::TimeTicks::Now();
  first_run::CreateSentinelIfNeeded();
#endif   

#if defined(ENABLE_BACKGROUND)
  browser_process_->profile_manager()->AutoloadProfiles();
#endif   

  TranslateService::Initialize();

  content::WebUIControllerFactory::RegisterFactory(
      ChromeWebUIControllerFactory::GetInstance());

#if !defined(DISABLE_NACL)
  NaClBrowserDelegateImpl* delegate =
      new NaClBrowserDelegateImpl(browser_process_->profile_manager());
  nacl::NaClBrowser::SetDelegate(delegate);
#endif   

  PostProfileInit();

  if (g_browser_process->gl_string_manager())
    g_browser_process->gl_string_manager()->Initialize();

  g_browser_process->gpu_mode_manager();

#if !defined(OS_ANDROID) && !defined(OS_CHROMEOS)
  if (first_run::IsChromeFirstRun()) {
    first_run::AutoImport(profile_,
                          master_prefs_->homepage_defined,
                          master_prefs_->do_import_items,
                          master_prefs_->dont_import_items,
                          master_prefs_->import_bookmarks_path);

    first_run::DoPostImportTasks(profile_,
                                 master_prefs_->make_chrome_default_for_user);

    if (!master_prefs_->suppress_first_run_default_browser_prompt) {
      browser_creator_->set_show_main_browser_window(
          !chrome::ShowFirstRunDefaultBrowserPrompt(profile_));
    } else {
      browser_creator_->set_is_default_browser_dialog_suppressed(true);
    }
  }
#endif   

#if defined(OS_WIN)
  ChromeBrowserMainPartsWin::PrepareRestartOnCrashEnviroment(
      parsed_command_line());

  if (base::win::GetVersion() >= base::win::VERSION_VISTA) {
    ChromeBrowserMainPartsWin::RegisterApplicationRestart(
        parsed_command_line());
  }

  if (NetworkProfileBubble::ShouldCheckNetworkProfile(profile_)) {
    BrowserThread::PostTask(BrowserThread::FILE, FROM_HERE,
        base::Bind(&NetworkProfileBubble::CheckNetworkProfile,
                   profile_->GetPath()));
  }
#endif   

#if defined(ENABLE_RLZ) && !defined(OS_CHROMEOS)
  PrefService* pref_service = profile_->GetPrefs();
  int ping_delay = first_run::IsChromeFirstRun() ? master_prefs_->ping_delay :
      pref_service->GetInteger(first_run::GetPingDelayPrefName().c_str());
  rlz::RLZTracker::SetRlzDelegate(
      make_scoped_ptr(new ChromeRLZTrackerDelegate));
  rlz::RLZTracker::InitRlzDelayed(
      first_run::IsChromeFirstRun(), ping_delay < 0,
      base::TimeDelta::FromMilliseconds(abs(ping_delay)),
      ChromeRLZTrackerDelegate::IsGoogleDefaultSearch(profile_),
      ChromeRLZTrackerDelegate::IsGoogleHomepage(profile_),
      ChromeRLZTrackerDelegate::IsGoogleInStartpages(profile_));
#endif   

  net::NetModule::SetResourceProvider(chrome_common_net::NetResourceProvider);

  browser_process_->intranet_redirect_detector();
  GoogleSearchCounter::RegisterForNotifications();

#if defined(ENABLE_PRINT_PREVIEW) && !defined(OFFICIAL_BUILD)
  if (parsed_command_line().HasSwitch(switches::kDebugPrint)) {
    base::FilePath path =
        parsed_command_line().GetSwitchValuePath(switches::kDebugPrint);
    printing::PrintedDocument::set_debug_dump_path(path);
  }
#endif   

  HandleTestParameters(parsed_command_line());
  browser_process_->metrics_service()->RecordBreakpadHasDebugger(
      base::debug::BeingDebugged());

  language_usage_metrics::LanguageUsageMetrics::RecordAcceptLanguages(
      profile_->GetPrefs()->GetString(prefs::kAcceptLanguages));
  language_usage_metrics::LanguageUsageMetrics::RecordApplicationLanguage(
      browser_process_->GetApplicationLocale());

  metrics::MetricsService::SetExecutionPhase(
      metrics::MetricsService::STARTUP_TIMEBOMB_ARM,
      g_browser_process->local_state());
  startup_watcher_->Arm(base::TimeDelta::FromSeconds(600));

#if !defined(OS_ANDROID)
  browser_process_->metrics_service()->LogNeedForCleanShutdown();
#endif   

#if defined(ENABLE_PRINT_PREVIEW)
  CloudPrintProxyServiceFactory::GetForProfile(profile_);
#endif   

  metrics::MetricsService::SetExecutionPhase(
      metrics::MetricsService::THREAD_WATCHER_START,
      g_browser_process->local_state());
  ThreadWatcherList::StartWatchingAll(parsed_command_line());

#if defined(OS_ANDROID)
  ThreadWatcherAndroid::RegisterApplicationStatusListener();
#endif   

#if !defined(DISABLE_NACL)
  BrowserThread::PostTask(
      BrowserThread::IO,
      FROM_HERE,
      base::Bind(nacl::NaClProcessHost::EarlyStartup));
#endif   

  PrefMetricsService::Factory::GetForProfile(profile_);

  PreBrowserStart();

  browser_process_->notification_ui_manager();

  translate::BrowserCldUtils::ConfigureDefaultDataProvider();

  if (!parsed_command_line().HasSwitch(switches::kDisableComponentUpdate))
    RegisterComponentsForUpdate();

#if defined(OS_ANDROID)
  chrome_variations::VariationsService* variations_service =
      browser_process_->variations_service();
  if (variations_service) {
    variations_service->set_policy_pref_service(profile_->GetPrefs());
  }
  translate::TranslateDownloadManager::RequestLanguageList(
      profile_->GetPrefs());

#else
  base::TimeTicks browser_open_start = base::TimeTicks::Now();

#if defined(OS_CHROMEOS)
  std::vector<Profile*> last_opened_profiles;
#else
  std::vector<Profile*> last_opened_profiles =
      g_browser_process->profile_manager()->GetLastOpenedProfiles();
#endif   

  UMA_HISTOGRAM_TIMES("Startup.PreMainMessageLoopRunImplStep2Time",
                      base::TimeTicks::Now() - start_time_step2);

  bool started = browser_creator_->Start(
      parsed_command_line(), base::FilePath(), profile_, last_opened_profiles);
  const base::TimeTicks start_time_step3 = base::TimeTicks::Now();
  if (started) {
#if defined(OS_WIN) || (defined(OS_LINUX) && !defined(OS_CHROMEOS))
    browser_process_->StartAutoupdateTimer();
#endif   

#if defined(OS_LINUX) && !defined(OS_CHROMEOS)
    upgrade_util::SaveLastModifiedTimeOfExe();
#endif   

    GoogleUpdateSettings::SetLastRunTime();

#if defined(OS_MACOSX)
    if (parameters().autorelease_pool)
      parameters().autorelease_pool->Recycle();
#endif   

    base::TimeDelta delay = base::TimeTicks::Now() - browser_open_start;
    UMA_HISTOGRAM_LONG_TIMES_100("Startup.BrowserOpenTabs", delay);

    if (parameters().ui_task == NULL) {
      chrome_variations::VariationsService* variations_service =
          browser_process_->variations_service();
      if (variations_service) {
        variations_service->StartRepeatedVariationsSeedFetch();

#if defined(OS_WIN)
        variations_service->StartGoogleUpdateRegistrySync();
#endif   
      }

      translate::TranslateDownloadManager::RequestLanguageList(
          profile_->GetPrefs());
    }

    run_message_loop_ = true;
  } else {
    run_message_loop_ = false;
  }
  browser_creator_.reset();

#if !defined(OS_LINUX) || defined(OS_CHROMEOS)   
  if (g_browser_process->metrics_service()->reporting_active())
    content::StartPowerUsageMonitor();
#endif   

  process_power_collector_.reset(new ProcessPowerCollector);
  process_power_collector_->Initialize();
#endif   

  PostBrowserStart();

  if (parameters().ui_task) {
    parameters().ui_task->Run();
    delete parameters().ui_task;
    run_message_loop_ = false;
  }
#if defined(OS_ANDROID)
  if (result_code_ <= 0) {
    RecordBrowserStartupTime();
  }
#endif   

#if !defined(OS_ANDROID)
  UMA_HISTOGRAM_TIMES("Startup.PreMainMessageLoopRunImplStep3Time",
                      base::TimeTicks::Now() - start_time_step3);
#endif   

  return result_code_;
}
