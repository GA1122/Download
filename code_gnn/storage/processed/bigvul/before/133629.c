void WebContentsImpl::SetIsLoading(RenderViewHost* render_view_host,
                                   bool is_loading,
                                   LoadNotificationDetails* details) {
  if (is_loading == is_loading_)
    return;

  if (!is_loading) {
    load_state_ = net::LoadStateWithParam(net::LOAD_STATE_IDLE,
                                          base::string16());
    load_state_host_.clear();
    upload_size_ = 0;
    upload_position_ = 0;
  }

  GetRenderManager()->SetIsLoading(is_loading);

  is_loading_ = is_loading;
  waiting_for_response_ = is_loading;

  if (delegate_)
    delegate_->LoadingStateChanged(this);
  NotifyNavigationStateChanged(INVALIDATE_TYPE_LOAD);

  std::string url = (details ? details->url.possibly_invalid_spec() : "NULL");
  if (is_loading) {
    TRACE_EVENT_ASYNC_BEGIN1("browser", "WebContentsImpl Loading", this,
                             "URL", url);
    FOR_EACH_OBSERVER(WebContentsObserver, observers_,
                      DidStartLoading(render_view_host));
  } else {
    TRACE_EVENT_ASYNC_END1("browser", "WebContentsImpl Loading", this,
                           "URL", url);
    FOR_EACH_OBSERVER(WebContentsObserver, observers_,
                      DidStopLoading(render_view_host));
  }

  int type = is_loading ? NOTIFICATION_LOAD_START : NOTIFICATION_LOAD_STOP;
  NotificationDetails det = NotificationService::NoDetails();
  if (details)
      det = Details<LoadNotificationDetails>(details);
  NotificationService::current()->Notify(
      type, Source<NavigationController>(&controller_), det);
}
