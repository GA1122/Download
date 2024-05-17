void ChromeNetworkDelegate::OnRequestWaitStateChange(
    const net::URLRequest& request,
    RequestWaitState state) {
  if (load_time_stats_)
    load_time_stats_->OnRequestWaitStateChange(request, state);
}
