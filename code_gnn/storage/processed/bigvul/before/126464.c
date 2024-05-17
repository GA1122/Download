void TabContentsContainerGtk::Observe(
    int type,
    const content::NotificationSource& source,
    const content::NotificationDetails& details) {
  DCHECK_EQ(content::NOTIFICATION_WEB_CONTENTS_DESTROYED, type);
  WebContentsDestroyed(content::Source<WebContents>(source).ptr());
}
