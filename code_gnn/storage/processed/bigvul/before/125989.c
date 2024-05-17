void InfoBarCountObserver::Observe(
    int type,
    const content::NotificationSource& source,
    const content::NotificationDetails& details) {
  DCHECK(type == chrome::NOTIFICATION_TAB_CONTENTS_INFOBAR_ADDED ||
         type == chrome::NOTIFICATION_TAB_CONTENTS_INFOBAR_REMOVED);
  CheckCount();
}
