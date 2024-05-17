void OneClickSigninHelper::OnStateChanged() {
  if (redirect_url_.is_empty())
    return;

  content::WebContents* contents = web_contents();
  Profile* profile =
      Profile::FromBrowserContext(contents->GetBrowserContext());
  ProfileSyncService* sync_service =
      ProfileSyncServiceFactory::GetForProfile(profile);

  if (sync_service->FirstSetupInProgress())
    return;

  if (sync_service->sync_initialized()) {
    contents->GetController().LoadURL(redirect_url_,
                                      content::Referrer(),
                                      content::PAGE_TRANSITION_AUTO_TOPLEVEL,
                                      std::string());
  }

  redirect_url_ = GURL();
  sync_service->RemoveObserver(this);
}
