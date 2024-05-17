void BrowserEventRouter::UnregisterForTabNotifications(WebContents* contents) {
  registrar_.Remove(this, content::NOTIFICATION_NAV_ENTRY_COMMITTED,
      content::Source<NavigationController>(&contents->GetController()));
  registrar_.Remove(this, content::NOTIFICATION_WEB_CONTENTS_DESTROYED,
      content::Source<WebContents>(contents));
}
