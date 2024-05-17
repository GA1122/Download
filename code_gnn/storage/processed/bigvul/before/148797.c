bool InterstitialPageImpl::OnMessageReceived(
    RenderFrameHostImpl* render_frame_host,
    const IPC::Message& message) {
  if (render_frame_host->GetRenderViewHost() != render_view_host_) {
    DCHECK(!render_view_host_)
        << "We expect an interstitial page to have only a single RVH";
    return false;
  }

  bool handled = true;
  IPC_BEGIN_MESSAGE_MAP_WITH_PARAM(InterstitialPageImpl, message,
                                   render_frame_host)
    IPC_MESSAGE_HANDLER(FrameHostMsg_DomOperationResponse,
                        OnDomOperationResponse)
    IPC_MESSAGE_UNHANDLED(handled = false)
  IPC_END_MESSAGE_MAP()

  return handled;
}
