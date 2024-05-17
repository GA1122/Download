void ChromeNetworkDelegate::OnURLRequestDestroyed(net::URLRequest* request) {
  ExtensionWebRequestEventRouter::GetInstance()->OnURLRequestDestroyed(
      profile_, request);
  if (load_time_stats_)
    load_time_stats_->OnURLRequestDestroyed(*request);
}
