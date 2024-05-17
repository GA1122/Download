void SessionChangeProcessor::StartObserving() {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  DCHECK(profile_);
  notification_registrar_.Add(this, content::NOTIFICATION_TAB_PARENTED,
      NotificationService::AllSources());
  notification_registrar_.Add(this, content::NOTIFICATION_TAB_CLOSED,
      NotificationService::AllSources());
  notification_registrar_.Add(this, content::NOTIFICATION_NAV_LIST_PRUNED,
      NotificationService::AllSources());
  notification_registrar_.Add(this, content::NOTIFICATION_NAV_ENTRY_CHANGED,
      NotificationService::AllSources());
  notification_registrar_.Add(this, content::NOTIFICATION_NAV_ENTRY_COMMITTED,
      NotificationService::AllSources());
  notification_registrar_.Add(this, chrome::NOTIFICATION_BROWSER_OPENED,
      NotificationService::AllBrowserContextsAndSources());
  notification_registrar_.Add(this,
      chrome::NOTIFICATION_TAB_CONTENTS_APPLICATION_EXTENSION_CHANGED,
      NotificationService::AllSources());
  notification_registrar_.Add(this,
      content::NOTIFICATION_LOAD_COMPLETED_MAIN_FRAME,
      NotificationService::AllBrowserContextsAndSources());
}
