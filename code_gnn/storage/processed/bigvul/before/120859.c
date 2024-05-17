OneClickSigninHelper::~OneClickSigninHelper() {
  content::WebContents* contents = web_contents();
  if (contents) {
    Profile* profile =
        Profile::FromBrowserContext(contents->GetBrowserContext());
    ProfileSyncService* sync_service =
        ProfileSyncServiceFactory::GetForProfile(profile);
    if (sync_service && sync_service->HasObserver(this))
      sync_service->RemoveObserver(this);
  }
}
