bool InlineSigninHelper::HandleCrossAccountError(
    const std::string& refresh_token,
    OneClickSigninSyncStarter::ConfirmationRequired confirmation_required,
    OneClickSigninSyncStarter::StartSyncMode start_mode) {
  if (signin_util::IsForceSigninEnabled())
    return false;

  std::string last_email =
      profile_->GetPrefs()->GetString(prefs::kGoogleServicesLastUsername);

  if (!IsCrossAccountError(profile_, email_, gaia_id_))
    return false;

  Browser* browser = chrome::FindLastActiveWithProfile(profile_);
  content::WebContents* web_contents =
      browser->tab_strip_model()->GetActiveWebContents();

  SigninEmailConfirmationDialog::AskForConfirmation(
      web_contents, profile_, last_email, email_,
      base::Bind(&InlineSigninHelper::ConfirmEmailAction,
                 base::Unretained(this), web_contents, refresh_token,
                 confirmation_required, start_mode));
  return true;
}
