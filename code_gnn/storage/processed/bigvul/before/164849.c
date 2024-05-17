  explicit HistoryObserver(Profile* profile) : profile_(profile) {
    DownloadCoreServiceFactory::GetForBrowserContext(profile_)
        ->GetDownloadHistory()
        ->AddObserver(this);
  }
