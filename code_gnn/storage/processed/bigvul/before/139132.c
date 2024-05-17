void RenderProcessHostImpl::CreateOffscreenCanvasCompositorFrameSinkProvider(
    blink::mojom::OffscreenCanvasCompositorFrameSinkProviderRequest request) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  if (!offscreen_canvas_provider_) {
    offscreen_canvas_provider_.reset(
        new OffscreenCanvasCompositorFrameSinkProviderImpl());
  }
  offscreen_canvas_provider_->Add(std::move(request));
}
