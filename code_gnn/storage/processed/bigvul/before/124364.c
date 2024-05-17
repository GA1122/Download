void BrowserEventRouter::Observe(int type,
                                 const content::NotificationSource& source,
                                 const content::NotificationDetails& details) {
  if (type == content::NOTIFICATION_NAV_ENTRY_COMMITTED) {
    NavigationController* source_controller =
        content::Source<NavigationController>(source).ptr();
    TabUpdated(source_controller->GetWebContents(), true);
  } else if (type == content::NOTIFICATION_WEB_CONTENTS_DESTROYED) {
    WebContents* contents = content::Source<WebContents>(source).ptr();
    registrar_.Remove(this, content::NOTIFICATION_NAV_ENTRY_COMMITTED,
        content::Source<NavigationController>(&contents->GetController()));
    registrar_.Remove(this, content::NOTIFICATION_WEB_CONTENTS_DESTROYED,
        content::Source<WebContents>(contents));
  } else {
    NOTREACHED();
  }
}
