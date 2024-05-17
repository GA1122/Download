void TabStripNotificationObserver::Observe(
    int type,
    const content::NotificationSource& source,
    const content::NotificationDetails& details) {
  DCHECK_EQ(notification_, type);
  if (type == chrome::NOTIFICATION_TAB_PARENTED) {
    ObserveTab(&content::Source<content::WebContents>(source)->GetController());
  } else if (type == content::NOTIFICATION_WEB_CONTENTS_DESTROYED) {
    ObserveTab(&content::Source<content::WebContents>(source)->GetController());
  } else {
    ObserveTab(content::Source<NavigationController>(source).ptr());
  }
  delete this;
}
