void TabLoader::StartLoading() {
  registrar_.Add(this, content::NOTIFICATION_RENDER_WIDGET_HOST_DID_PAINT,
                 content::NotificationService::AllSources());
#if defined(OS_CHROMEOS)
  if (!net::NetworkChangeNotifier::IsOffline()) {
    loading_ = true;
    LoadNextTab();
  } else {
    net::NetworkChangeNotifier::AddOnlineStateObserver(this);
  }
#else
  loading_ = true;
  LoadNextTab();
#endif
}
