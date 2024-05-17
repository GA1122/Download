void BindMediaStreamDeviceObserverRequest(
    int render_process_id,
    int render_frame_id,
    blink::mojom::MediaStreamDeviceObserverRequest request) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  RenderFrameHost* render_frame_host =
      RenderFrameHost::FromID(render_process_id, render_frame_id);
  if (render_frame_host)
    render_frame_host->GetRemoteInterfaces()->GetInterface(std::move(request));
}
