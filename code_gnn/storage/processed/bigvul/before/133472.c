OobeUI::OobeUI(content::WebUI* web_ui, const GURL& url)
    : WebUIController(web_ui),
      core_handler_(nullptr),
      network_dropdown_handler_(nullptr),
      update_view_(nullptr),
      network_view_(nullptr),
      debugging_screen_actor_(nullptr),
      eula_view_(nullptr),
      reset_view_(nullptr),
      hid_detection_view_(nullptr),
      autolaunch_screen_actor_(nullptr),
      kiosk_enable_screen_actor_(nullptr),
      wrong_hwid_screen_actor_(nullptr),
      auto_enrollment_check_screen_actor_(nullptr),
      supervised_user_creation_screen_actor_(nullptr),
      app_launch_splash_screen_actor_(nullptr),
      controller_pairing_screen_actor_(nullptr),
      host_pairing_screen_actor_(nullptr),
      device_disabled_screen_actor_(nullptr),
      error_screen_handler_(nullptr),
      signin_screen_handler_(nullptr),
      terms_of_service_screen_actor_(nullptr),
      user_image_view_(nullptr),
      kiosk_app_menu_handler_(nullptr),
      current_screen_(SCREEN_UNKNOWN),
      previous_screen_(SCREEN_UNKNOWN),
      ready_(false) {
  display_type_ = GetDisplayType(url);
  InitializeScreenMaps();

  network_state_informer_ = new NetworkStateInformer();
  network_state_informer_->Init();

  core_handler_ = new CoreOobeHandler(this);
  AddScreenHandler(core_handler_);
  core_handler_->SetDelegate(this);

  network_dropdown_handler_ = new NetworkDropdownHandler();
  AddScreenHandler(network_dropdown_handler_);

  UpdateScreenHandler* update_screen_handler = new UpdateScreenHandler();
  update_view_ = update_screen_handler;
  AddScreenHandler(update_screen_handler);

  if (display_type_ == kOobeDisplay) {
    NetworkScreenHandler* network_screen_handler =
        new NetworkScreenHandler(core_handler_);
    network_view_ = network_screen_handler;
    AddScreenHandler(network_screen_handler);
  }

  EnableDebuggingScreenHandler* debugging_screen_handler =
      new EnableDebuggingScreenHandler();
  debugging_screen_actor_ = debugging_screen_handler;
  AddScreenHandler(debugging_screen_handler);

  EulaScreenHandler* eula_screen_handler = new EulaScreenHandler(core_handler_);
  eula_view_ = eula_screen_handler;
  AddScreenHandler(eula_screen_handler);

  ResetScreenHandler* reset_screen_handler = new ResetScreenHandler();
  reset_view_ = reset_screen_handler;
  AddScreenHandler(reset_screen_handler);

  KioskAutolaunchScreenHandler* autolaunch_screen_handler =
      new KioskAutolaunchScreenHandler();
  autolaunch_screen_actor_ = autolaunch_screen_handler;
  AddScreenHandler(autolaunch_screen_handler);

  KioskEnableScreenHandler* kiosk_enable_screen_handler =
      new KioskEnableScreenHandler();
  kiosk_enable_screen_actor_ = kiosk_enable_screen_handler;
  AddScreenHandler(kiosk_enable_screen_handler);

  SupervisedUserCreationScreenHandler* supervised_user_creation_screen_handler =
      new SupervisedUserCreationScreenHandler();
  supervised_user_creation_screen_actor_ =
      supervised_user_creation_screen_handler;
  AddScreenHandler(supervised_user_creation_screen_handler);

  WrongHWIDScreenHandler* wrong_hwid_screen_handler =
      new WrongHWIDScreenHandler();
  wrong_hwid_screen_actor_ = wrong_hwid_screen_handler;
  AddScreenHandler(wrong_hwid_screen_handler);

  AutoEnrollmentCheckScreenHandler* auto_enrollment_check_screen_handler =
      new AutoEnrollmentCheckScreenHandler();
  auto_enrollment_check_screen_actor_ = auto_enrollment_check_screen_handler;
  AddScreenHandler(auto_enrollment_check_screen_handler);

  HIDDetectionScreenHandler* hid_detection_screen_handler =
      new HIDDetectionScreenHandler(core_handler_);
  hid_detection_view_ = hid_detection_screen_handler;
  AddScreenHandler(hid_detection_screen_handler);

  error_screen_handler_ = new ErrorScreenHandler();
  AddScreenHandler(error_screen_handler_);
  network_dropdown_handler_->AddObserver(error_screen_handler_);

  error_screen_.reset(new ErrorScreen(nullptr, error_screen_handler_));
  NetworkErrorModel* network_error_model = error_screen_.get();

  EnrollmentScreenHandler* enrollment_screen_handler =
      new EnrollmentScreenHandler(network_state_informer_, network_error_model);
  enrollment_screen_actor_ = enrollment_screen_handler;
  AddScreenHandler(enrollment_screen_handler);

  TermsOfServiceScreenHandler* terms_of_service_screen_handler =
      new TermsOfServiceScreenHandler(core_handler_);
  terms_of_service_screen_actor_ = terms_of_service_screen_handler;
  AddScreenHandler(terms_of_service_screen_handler);

  UserImageScreenHandler* user_image_screen_handler =
      new UserImageScreenHandler();
  user_image_view_ = user_image_screen_handler;
  AddScreenHandler(user_image_screen_handler);

  policy::ConsumerManagementService* consumer_management =
      g_browser_process->platform_part()->browser_policy_connector_chromeos()->
          GetConsumerManagementService();

  user_board_screen_handler_ = new UserBoardScreenHandler();
  AddScreenHandler(user_board_screen_handler_);

  gaia_screen_handler_ =
      new GaiaScreenHandler(
          core_handler_, network_state_informer_, consumer_management);
  AddScreenHandler(gaia_screen_handler_);

  signin_screen_handler_ =
      new SigninScreenHandler(network_state_informer_, network_error_model,
                              core_handler_, gaia_screen_handler_);
  AddScreenHandler(signin_screen_handler_);

  AppLaunchSplashScreenHandler* app_launch_splash_screen_handler =
      new AppLaunchSplashScreenHandler(network_state_informer_,
                                       network_error_model);
  AddScreenHandler(app_launch_splash_screen_handler);
  app_launch_splash_screen_actor_ = app_launch_splash_screen_handler;

  if (display_type_ == kOobeDisplay) {
    ControllerPairingScreenHandler* handler =
        new ControllerPairingScreenHandler();
    controller_pairing_screen_actor_ = handler;
    AddScreenHandler(handler);
  }

  if (display_type_ == kOobeDisplay) {
    HostPairingScreenHandler* handler = new HostPairingScreenHandler();
    host_pairing_screen_actor_ = handler;
    AddScreenHandler(handler);
  }

  DeviceDisabledScreenHandler* device_disabled_screen_handler =
      new DeviceDisabledScreenHandler;
  device_disabled_screen_actor_ = device_disabled_screen_handler;
  AddScreenHandler(device_disabled_screen_handler);

  kiosk_app_menu_handler_ = new KioskAppMenuHandler(network_state_informer_);
  web_ui->AddMessageHandler(kiosk_app_menu_handler_);

  base::DictionaryValue localized_strings;
  GetLocalizedStrings(&localized_strings);

  Profile* profile = Profile::FromWebUI(web_ui);
  ThemeSource* theme = new ThemeSource(profile);
  content::URLDataSource::Add(profile, theme);

  AboutUIHTMLSource* about_source =
      new AboutUIHTMLSource(chrome::kChromeUITermsHost, profile);
  content::URLDataSource::Add(profile, about_source);

  content::WebUIDataSource::Add(profile,
                                CreateOobeUIDataSource(localized_strings,
                                                       display_type_));

  options::UserImageSource* user_image_source =
      new options::UserImageSource();
  content::URLDataSource::Add(profile, user_image_source);

  content::WebContents* contents = web_ui->GetWebContents();
  extensions::TabHelper::CreateForWebContents(contents);
}
