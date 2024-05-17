void RenderViewImpl::OnSetZoomLevelForLoadingURL(const GURL& url,
                                                 double zoom_level) {
#if !defined(OS_ANDROID)
  host_zoom_levels_[url] = zoom_level;
#endif
}
