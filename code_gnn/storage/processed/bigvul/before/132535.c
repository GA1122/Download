LayoutTestContentBrowserClient::LayoutTestContentBrowserClient() {
  DCHECK(!g_layout_test_browser_client);

  layout_test_notification_manager_.reset(new LayoutTestNotificationManager());

  g_layout_test_browser_client = this;
}
