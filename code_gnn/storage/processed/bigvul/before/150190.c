  ProfileLaunchObserver() {
    registrar_.Add(this, chrome::NOTIFICATION_PROFILE_DESTROYED,
                   content::NotificationService::AllSources());
    BrowserList::AddObserver(this);
  }
