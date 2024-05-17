void InlineLoginHandlerImpl::SyncStarterCallback(
    OneClickSigninSyncStarter::SyncSetupResult result) {
  content::WebContents* contents = web_ui()->GetWebContents();

  if (contents->GetController().GetPendingEntry()) {
    return;
  }

  const GURL& current_url = contents->GetLastCommittedURL();
  signin_metrics::AccessPoint access_point =
      signin::GetAccessPointForPromoURL(current_url);
  bool auto_close = signin::IsAutoCloseEnabledInURL(current_url);

  if (result == OneClickSigninSyncStarter::SYNC_SETUP_FAILURE) {
    RedirectToNtpOrAppsPage(contents, access_point);
  } else if (auto_close) {
    bool show_account_management = ShouldShowAccountManagement(
        current_url, AccountConsistencyModeManager::IsMirrorEnabledForProfile(
                         Profile::FromWebUI(web_ui())));
    base::ThreadTaskRunnerHandle::Get()->PostTask(
        FROM_HERE,
        base::BindOnce(&InlineLoginHandlerImpl::CloseTab,
                       weak_factory_.GetWeakPtr(), show_account_management));
  } else {
    RedirectToNtpOrAppsPageIfNecessary(contents, access_point);
  }
}
