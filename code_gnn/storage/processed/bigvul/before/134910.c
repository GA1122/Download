void ChromeBrowserMainPartsChromeos::PostMainMessageLoopRun() {
  chromeos::ResourceReporter::GetInstance()->StopMonitoring();

  BootTimesRecorder::Get()->AddLogoutTimeMarker("UIMessageLoopEnded", true);

  if (lock_screen_apps_state_controller_)
    lock_screen_apps_state_controller_->Shutdown();

  NoteTakingHelper::Shutdown();

  arc_service_launcher_->Shutdown();

  arc_voice_interaction_controller_client_.reset();

  shutdown_policy_forwarder_.reset();

  KioskModeIdleAppNameNotification::Shutdown();

  if (UpgradeDetectorChromeos::GetInstance())
    UpgradeDetectorChromeos::GetInstance()->Shutdown();

  if (NetworkChangeNotifierFactoryChromeos::GetInstance())
    NetworkChangeNotifierFactoryChromeos::GetInstance()->Shutdown();

  DeviceSettingsService::Get()->UnsetSessionManager();

  network_pref_state_observer_.reset();
  power_metrics_reporter_.reset();
  renderer_freezer_.reset();
  wake_on_wifi_manager_.reset();
  network_throttling_observer_.reset();
  ScreenLocker::ShutDownClass();
  low_disk_notification_.reset();
  user_activity_controller_.reset();
  adaptive_screen_brightness_manager_.reset();

  idle_action_warning_observer_.reset();

  if (chromeos::GetAshConfig() != ash::Config::MASH)
    MagnificationManager::Shutdown();

  media::SoundsManager::Shutdown();

  system::StatisticsProvider::GetInstance()->Shutdown();

  chromeos::DemoSession::ShutDownIfInitialized();

  g_browser_process->platform_part()->user_manager()->Shutdown();

  g_browser_process->platform_part()->ShutdownDeviceDisablingManager();

  g_browser_process->platform_part()->ShutdownAutomaticRebootManager();

  KioskAppManager::Shutdown();

  UserSessionManager::GetInstance()->Shutdown();

  g_browser_process->platform_part()
      ->browser_policy_connector_chromeos()
      ->PreShutdown();

  ChromeBrowserMainPartsLinux::PostMainMessageLoopRun();

  arc_kiosk_app_manager_.reset();

  arc_service_launcher_.reset();

  if (chromeos::GetAshConfig() != ash::Config::MASH)
    AccessibilityManager::Shutdown();

  input_method::Shutdown();

  DeviceOAuth2TokenServiceFactory::Shutdown();

  content::MediaCaptureDevices::GetInstance()->RemoveAllVideoCaptureObservers();

  CrasAudioHandler::Shutdown();

  quirks::QuirksManager::Shutdown();

  network_portal_detector::Shutdown();

  g_browser_process->platform_part()->ShutdownSessionManager();
  g_browser_process->platform_part()->DestroyChromeUserManager();
}
