void RenderViewHostImpl::ClosePage() {
  is_waiting_for_unload_ack_ = true;
  StartHangMonitorTimeout(TimeDelta::FromMilliseconds(kUnloadTimeoutMS));

  if (IsRenderViewLive()) {
    increment_in_flight_event_count();

    NotificationService::current()->Notify(
        NOTIFICATION_RENDER_VIEW_HOST_WILL_CLOSE_RENDER_VIEW,
        Source<RenderViewHost>(this),
        NotificationService::NoDetails());

    Send(new ViewMsg_ClosePage(GetRoutingID()));
  } else {
    ClosePageIgnoringUnloadEvents();
  }
}
