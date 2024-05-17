void OobeUI::InitializeScreenMaps() {
  screen_names_.resize(SCREEN_UNKNOWN);
  screen_names_[SCREEN_OOBE_HID_DETECTION] = kScreenOobeHIDDetection;
  screen_names_[SCREEN_OOBE_NETWORK] = kScreenOobeNetwork;
  screen_names_[SCREEN_OOBE_EULA] = kScreenOobeEula;
  screen_names_[SCREEN_OOBE_UPDATE] = kScreenOobeUpdate;
  screen_names_[SCREEN_OOBE_ENROLLMENT] = kScreenOobeEnrollment;
  screen_names_[SCREEN_OOBE_ENABLE_DEBUGGING] = kScreenOobeEnableDebugging;
  screen_names_[SCREEN_OOBE_RESET] = kScreenOobeReset;
  screen_names_[SCREEN_GAIA_SIGNIN] = kScreenGaiaSignin;
  screen_names_[SCREEN_ACCOUNT_PICKER] = kScreenAccountPicker;
  screen_names_[SCREEN_KIOSK_AUTOLAUNCH] = kScreenKioskAutolaunch;
  screen_names_[SCREEN_KIOSK_ENABLE] = kScreenKioskEnable;
  screen_names_[SCREEN_ERROR_MESSAGE] = kScreenErrorMessage;
  screen_names_[SCREEN_USER_IMAGE_PICKER] = kScreenUserImagePicker;
  screen_names_[SCREEN_TPM_ERROR] = kScreenTpmError;
  screen_names_[SCREEN_PASSWORD_CHANGED] = kScreenPasswordChanged;
  screen_names_[SCREEN_CREATE_SUPERVISED_USER_FLOW] =
      kScreenSupervisedUserCreationFlow;
  screen_names_[SCREEN_TERMS_OF_SERVICE] = kScreenTermsOfService;
  screen_names_[SCREEN_WRONG_HWID] = kScreenWrongHWID;
  screen_names_[SCREEN_AUTO_ENROLLMENT_CHECK] = kScreenAutoEnrollmentCheck;
  screen_names_[SCREEN_APP_LAUNCH_SPLASH] = kScreenAppLaunchSplash;
  screen_names_[SCREEN_CONFIRM_PASSWORD] = kScreenConfirmPassword;
  screen_names_[SCREEN_FATAL_ERROR] = kScreenFatalError;
  screen_names_[SCREEN_OOBE_CONTROLLER_PAIRING] = kScreenControllerPairing;
  screen_names_[SCREEN_OOBE_HOST_PAIRING] = kScreenHostPairing;
  screen_names_[SCREEN_DEVICE_DISABLED] = kScreenDeviceDisabled;

  screen_ids_.clear();
  for (size_t i = 0; i < screen_names_.size(); ++i)
    screen_ids_[screen_names_[i]] = static_cast<Screen>(i);
}
