void ScreenOrientationDispatcherHost::NotifyLockError(
    int request_id, blink::WebLockOrientationError error) {
  RenderFrameHost* render_frame_host =
      GetRenderFrameHostForRequestID(request_id);
  if (!render_frame_host)
    return;

  render_frame_host->Send(new ScreenOrientationMsg_LockError(
      render_frame_host->GetRoutingID(), request_id, error));
  ResetCurrentLock();
}
