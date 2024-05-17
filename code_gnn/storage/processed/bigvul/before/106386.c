OfflineLoadPage::OfflineLoadPage(TabContents* tab_contents,
                                 const GURL& url,
                                 Delegate* delegate)
    : ChromeInterstitialPage(tab_contents, true, url),
      delegate_(delegate),
      proceeded_(false),
      ALLOW_THIS_IN_INITIALIZER_LIST(method_factory_(this)),
      in_test_(false) {
  registrar_.Add(this, NotificationType::NETWORK_STATE_CHANGED,
                  NotificationService::AllSources());
 }
