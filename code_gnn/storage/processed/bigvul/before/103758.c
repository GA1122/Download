void RenderThread::OnSetZoomLevelForCurrentURL(const GURL& url,
                                               double zoom_level) {
  RenderViewZoomer zoomer(url, zoom_level);
  RenderView::ForEach(&zoomer);
}
