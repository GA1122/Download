void LayoutTestContentRendererClient::RenderFrameCreated(
    RenderFrame* render_frame) {
  new LayoutTestRenderFrameObserver(render_frame);
}
