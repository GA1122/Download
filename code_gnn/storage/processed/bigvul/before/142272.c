ChromePasswordManagerClient::ChromePasswordManagerClient(
    content::WebContents* web_contents,
    autofill::AutofillClient* autofill_client)
    : content::WebContentsObserver(web_contents),
      profile_(Profile::FromBrowserContext(web_contents->GetBrowserContext())),
      password_manager_(this),
#if !defined(OS_ANDROID)
      password_reuse_detection_manager_(this),
#endif
      driver_factory_(nullptr),
      content_credential_manager_(this),
      password_generation_driver_bindings_(web_contents, this),
      password_manager_driver_bindings_(web_contents, this),
      observer_(nullptr),
      credentials_filter_(
          this,
          base::BindRepeating(&GetSyncService, profile_),
          base::BindRepeating(&GetIdentityManagerForOriginalProfile, profile_)),
      helper_(this) {
  ContentPasswordManagerDriverFactory::CreateForWebContents(web_contents, this,
                                                            autofill_client);
  driver_factory_ =
      ContentPasswordManagerDriverFactory::FromWebContents(web_contents);
  log_manager_ = password_manager::LogManager::Create(
      password_manager::PasswordManagerInternalsServiceFactory::
          GetForBrowserContext(profile_),
      base::Bind(
          &ContentPasswordManagerDriverFactory::RequestSendLoggingAvailability,
          base::Unretained(driver_factory_)));

  saving_and_filling_passwords_enabled_.Init(
      password_manager::prefs::kCredentialsEnableService, GetPrefs());
  static base::NoDestructor<password_manager::StoreMetricsReporter> reporter(
      *saving_and_filling_passwords_enabled_, this, GetSyncService(profile_),
      GetIdentityManagerForOriginalProfile(profile_), GetPrefs());
  driver_factory_->RequestSendLoggingAvailability();
}
