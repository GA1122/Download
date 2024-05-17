void InlineSigninHelper::CreateSyncStarter(
    Browser* browser,
    const GURL& current_url,
    const std::string& refresh_token,
    OneClickSigninSyncStarter::ProfileMode profile_mode,
    OneClickSigninSyncStarter::StartSyncMode start_mode,
    OneClickSigninSyncStarter::ConfirmationRequired confirmation_required) {
  new OneClickSigninSyncStarter(
      profile_, browser, gaia_id_, email_, password_, refresh_token,
      signin::GetAccessPointForPromoURL(current_url),
      signin::GetSigninReasonForPromoURL(current_url), profile_mode, start_mode,
      confirmation_required,
      base::Bind(&InlineLoginHandlerImpl::SyncStarterCallback, handler_));
}
