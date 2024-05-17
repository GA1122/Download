void PanelBrowserView::Observe(
    int type,
    const content::NotificationSource& source,
    const content::NotificationDetails& details) {
  switch (type) {
    case chrome::NOTIFICATION_WINDOW_CLOSED:
      panel_->OnNativePanelClosed();
      break;
    default:
      NOTREACHED();
  }
}
