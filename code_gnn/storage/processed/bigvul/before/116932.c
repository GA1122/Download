TabLoader::~TabLoader() {
  DCHECK((got_first_paint_ || render_widget_hosts_to_paint_.empty()) &&
          tabs_loading_.empty() && tabs_to_load_.empty());
  net::NetworkChangeNotifier::RemoveOnlineStateObserver(this);
}
