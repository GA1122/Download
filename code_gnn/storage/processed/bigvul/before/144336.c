void ShowLoginWizard(OobeScreen first_screen) {
  if (browser_shutdown::IsTryingToQuit())
    return;

  VLOG(1) << "Showing OOBE screen: " << GetOobeScreenName(first_screen);

  input_method::InputMethodManager* manager =
      input_method::InputMethodManager::Get();

  if (g_browser_process && g_browser_process->local_state()) {
    manager->GetActiveIMEState()->SetInputMethodLoginDefault();

    PrefService* prefs = g_browser_process->local_state();
    system::InputDeviceSettings::Get()->SetPrimaryButtonRight(
        prefs->GetBoolean(prefs::kOwnerPrimaryMouseButtonRight));
    system::InputDeviceSettings::Get()->SetTapToClick(
        prefs->GetBoolean(ash::prefs::kOwnerTapToClickEnabled));
  }
  system::InputDeviceSettings::Get()->SetNaturalScroll(
      base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kNaturalScrollDefault));

  auto session_state = session_manager::SessionState::OOBE;
  if (IsOobeComplete() || first_screen == OobeScreen::SCREEN_SPECIAL_LOGIN)
    session_state = session_manager::SessionState::LOGIN_PRIMARY;
  session_manager::SessionManager::Get()->SetSessionState(session_state);

  bool show_app_launch_splash_screen =
      (first_screen == OobeScreen::SCREEN_APP_LAUNCH_SPLASH);
  if (show_app_launch_splash_screen) {
    const std::string& auto_launch_app_id =
        KioskAppManager::Get()->GetAutoLaunchApp();
    const bool diagnostic_mode = false;
    const bool auto_launch = true;
    auto* display_host = new LoginDisplayHostWebUI();
    display_host->StartAppLaunch(auto_launch_app_id, diagnostic_mode,
                                 auto_launch);
    return;
  }

  const policy::EnrollmentConfig enrollment_config =
      g_browser_process->platform_part()
          ->browser_policy_connector_chromeos()
          ->GetPrescribedEnrollmentConfig();
  if (enrollment_config.should_enroll() &&
      first_screen == OobeScreen::SCREEN_UNKNOWN) {
    auto* display_host = new LoginDisplayHostWebUI();
    display_host->StartWizard(OobeScreen::SCREEN_OOBE_WELCOME);
    return;
  }

  if (StartupUtils::IsEulaAccepted()) {
    DelayNetworkCall(
        base::TimeDelta::FromMilliseconds(kDefaultNetworkRetryDelayMS),
        ServicesCustomizationDocument::GetInstance()
            ->EnsureCustomizationAppliedClosure());

    g_browser_process->platform_part()
        ->GetTimezoneResolverManager()
        ->UpdateTimezoneResolver();
  }

  PrefService* prefs = g_browser_process->local_state();
  std::string current_locale = prefs->GetString(prefs::kApplicationLocale);
  language::ConvertToActualUILocale(&current_locale);
  VLOG(1) << "Current locale: " << current_locale;

  if (ShouldShowSigninScreen(first_screen)) {
    std::string switch_locale = GetManagedLoginScreenLocale();
    if (switch_locale == current_locale)
      switch_locale.clear();

    std::unique_ptr<ShowLoginWizardSwitchLanguageCallbackData> data =
        std::make_unique<ShowLoginWizardSwitchLanguageCallbackData>(
            first_screen, nullptr);
    TriggerShowLoginWizardFinish(switch_locale, std::move(data));
    return;
  }

  const StartupCustomizationDocument* startup_manifest =
      StartupCustomizationDocument::GetInstance();

  const std::string& locale = startup_manifest->initial_locale_default();

  const std::string& layout = startup_manifest->keyboard_layout();
  VLOG(1) << "Initial locale: " << locale << "keyboard layout " << layout;

  manager->GetActiveIMEState()->SetInputMethodLoginDefaultFromVPD(locale,
                                                                  layout);

  std::unique_ptr<ShowLoginWizardSwitchLanguageCallbackData> data(
      new ShowLoginWizardSwitchLanguageCallbackData(first_screen,
                                                    startup_manifest));

  if (!current_locale.empty() || locale.empty()) {
    TriggerShowLoginWizardFinish(std::string(), std::move(data));
    return;
  }

  prefs->SetString(prefs::kApplicationLocale, locale);
  StartupUtils::SetInitialLocale(locale);

  TriggerShowLoginWizardFinish(locale, std::move(data));
}
