WebBluetoothServiceImpl::WebBluetoothServiceImpl(
    RenderFrameHost* render_frame_host,
    blink::mojom::WebBluetoothServiceRequest request)
    : WebContentsObserver(WebContents::FromRenderFrameHost(render_frame_host)),
      connected_devices_(new FrameConnectedBluetoothDevices(render_frame_host)),
      render_frame_host_(render_frame_host),
      binding_(this, std::move(request)) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  CHECK(web_contents());
}
