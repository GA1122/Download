bool RenderThread::Send(IPC::Message* msg) {
  bool pumping_events = false;
  if (msg->is_sync()) {
    if (msg->is_caller_pumping_messages()) {
      pumping_events = true;
    } else {
      if ((msg->type() == ViewHostMsg_GetCookies::ID ||
           msg->type() == ViewHostMsg_GetRawCookies::ID ||
           msg->type() == ViewHostMsg_CookiesEnabled::ID) &&
          content::GetContentClient()->renderer()->
              ShouldPumpEventsDuringCookieMessage()) {
        pumping_events = true;
      }
    }
  }

  bool suspend_webkit_shared_timer = true;   
  std::swap(suspend_webkit_shared_timer, suspend_webkit_shared_timer_);

  bool notify_webkit_of_modal_loop = true;   
  std::swap(notify_webkit_of_modal_loop, notify_webkit_of_modal_loop_);

  gfx::NativeViewId host_window = 0;

  if (pumping_events) {
    if (suspend_webkit_shared_timer)
      webkit_client_->SuspendSharedTimer();

    if (notify_webkit_of_modal_loop)
      WebView::willEnterModalLoop();

    RenderWidget* widget =
        static_cast<RenderWidget*>(ResolveRoute(msg->routing_id()));
    if (widget) {
      host_window = widget->host_window();
      PluginChannelHost::Broadcast(
          new PluginMsg_SignalModalDialogEvent(host_window));
    }
  }

  bool rv = ChildThread::Send(msg);

  if (pumping_events) {
    if (host_window) {
      PluginChannelHost::Broadcast(
          new PluginMsg_ResetModalDialogEvent(host_window));
    }

    if (notify_webkit_of_modal_loop)
      WebView::didExitModalLoop();

    if (suspend_webkit_shared_timer)
      webkit_client_->ResumeSharedTimer();
  }

  return rv;
}
