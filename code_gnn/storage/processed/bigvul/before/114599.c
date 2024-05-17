void RenderThreadImpl::OnSetZoomLevelForCurrentURL(const std::string& host,
                                                   double zoom_level) {
  RenderViewZoomer zoomer(host, zoom_level);
  content::RenderView::ForEach(&zoomer);
}
