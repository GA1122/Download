RenderFrameHost* ConvertToRenderFrameHost(RenderViewHost* render_view_host) {
  return render_view_host->GetMainFrame();
}
