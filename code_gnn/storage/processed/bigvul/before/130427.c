void ThreadWatcherObserver::SetupNotifications(
    const base::TimeDelta& wakeup_interval) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  ThreadWatcherObserver* observer = new ThreadWatcherObserver(wakeup_interval);
  observer->registrar_.Add(
      observer,
      chrome::NOTIFICATION_BROWSER_OPENED,
      content::NotificationService::AllBrowserContextsAndSources());
  observer->registrar_.Add(observer,
                           chrome::NOTIFICATION_BROWSER_CLOSED,
                           content::NotificationService::AllSources());
  observer->registrar_.Add(observer,
                           chrome::NOTIFICATION_TAB_PARENTED,
                           content::NotificationService::AllSources());
  observer->registrar_.Add(observer,
                           chrome::NOTIFICATION_TAB_CLOSING,
                           content::NotificationService::AllSources());
  observer->registrar_.Add(observer,
                           content::NOTIFICATION_LOAD_START,
                           content::NotificationService::AllSources());
  observer->registrar_.Add(observer,
                           content::NOTIFICATION_LOAD_STOP,
                           content::NotificationService::AllSources());
  observer->registrar_.Add(observer,
                           content::NOTIFICATION_RENDERER_PROCESS_CLOSED,
                           content::NotificationService::AllSources());
  observer->registrar_.Add(observer,
                           content::NOTIFICATION_RENDER_WIDGET_HOST_HANG,
                           content::NotificationService::AllSources());
  observer->omnibox_url_opened_subscription_ =
      OmniboxEventGlobalTracker::GetInstance()->RegisterCallback(
          base::Bind(&ThreadWatcherObserver::OnURLOpenedFromOmnibox,
                     base::Unretained(observer)));
}
