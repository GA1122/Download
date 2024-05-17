void RenderViewImpl::OnSetZoomLevelForLoadingURL(const GURL& url,
                                                 double zoom_level) {
  host_zoom_levels_[url] = zoom_level;
}
