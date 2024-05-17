void GDataFileSystem::Observe(int type,
                              const content::NotificationSource& source,
                              const content::NotificationDetails& details) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  if (type == chrome::NOTIFICATION_PREF_CHANGED) {
    PrefService* pref_service = profile_->GetPrefs();
    std::string* pref_name = content::Details<std::string>(details).ptr();
    if (*pref_name == prefs::kDisableGDataHostedFiles) {
      SetHideHostedDocuments(
          pref_service->GetBoolean(prefs::kDisableGDataHostedFiles));
    }
  } else {
    NOTREACHED();
  }
}
