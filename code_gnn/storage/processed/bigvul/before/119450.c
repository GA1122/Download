void ScreenOrientationDispatcherHost::NotifyLockSuccess(int request_id) {
  RenderFrameHost* render_frame_host =
      GetRenderFrameHostForRequestID(request_id);
  if (!render_frame_host)
    return;

  render_frame_host->Send(new ScreenOrientationMsg_LockSuccess(
      render_frame_host->GetRoutingID(), request_id));
  ResetCurrentLock();
}
