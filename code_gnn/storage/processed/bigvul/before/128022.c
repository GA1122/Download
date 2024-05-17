void OnIoThreadClientReady(content::RenderFrameHost* rfh) {
  int render_process_id = rfh->GetProcess()->GetID();
  int render_frame_id = rfh->GetRoutingID();
  AwResourceDispatcherHostDelegate::OnIoThreadClientReady(
      render_process_id, render_frame_id);
}
