void BrowserWindowGtk::Observe(int type,
                               const content::NotificationSource& source,
                               const content::NotificationDetails& details) {
  DCHECK_EQ(chrome::NOTIFICATION_PROFILE_CACHED_INFO_CHANGED, type);
  gtk_util::SetWindowIcon(window_, browser_->profile());
}
