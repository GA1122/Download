void InlineSigninHelper::ConfirmEmailAction(
    content::WebContents* web_contents,
    const std::string& refresh_token,
    OneClickSigninSyncStarter::ConfirmationRequired confirmation_required,
    OneClickSigninSyncStarter::StartSyncMode start_mode,
    SigninEmailConfirmationDialog::Action action) {
  Browser* browser = chrome::FindLastActiveWithProfile(profile_);
  switch (action) {
    case SigninEmailConfirmationDialog::CREATE_NEW_USER:
      base::RecordAction(
          base::UserMetricsAction("Signin_ImportDataPrompt_DontImport"));
      CreateSyncStarter(browser, current_url_, refresh_token,
                        OneClickSigninSyncStarter::NEW_PROFILE, start_mode,
                        confirmation_required);
      break;
    case SigninEmailConfirmationDialog::START_SYNC:
      base::RecordAction(
          base::UserMetricsAction("Signin_ImportDataPrompt_ImportData"));
      CreateSyncStarter(browser, current_url_, refresh_token,
                        OneClickSigninSyncStarter::CURRENT_PROFILE, start_mode,
                        confirmation_required);
      break;
    case SigninEmailConfirmationDialog::CLOSE:
      base::RecordAction(
          base::UserMetricsAction("Signin_ImportDataPrompt_Cancel"));
      if (handler_) {
        handler_->SyncStarterCallback(
            OneClickSigninSyncStarter::SYNC_SETUP_FAILURE);
      }
      break;
    default:
      DCHECK(false) << "Invalid action";
  }
  base::ThreadTaskRunnerHandle::Get()->DeleteSoon(FROM_HERE, this);
}
