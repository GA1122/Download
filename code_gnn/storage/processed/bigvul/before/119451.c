bool ScreenOrientationDispatcherHost::OnMessageReceived(
    const IPC::Message& message,
    RenderFrameHost* render_frame_host) {
  bool handled = true;

  IPC_BEGIN_MESSAGE_MAP_WITH_PARAM(ScreenOrientationDispatcherHost, message,
                                   render_frame_host)
    IPC_MESSAGE_HANDLER(ScreenOrientationHostMsg_LockRequest, OnLockRequest)
    IPC_MESSAGE_HANDLER(ScreenOrientationHostMsg_Unlock, OnUnlockRequest)
    IPC_MESSAGE_UNHANDLED(handled = false)
  IPC_END_MESSAGE_MAP()

  return handled;
}
