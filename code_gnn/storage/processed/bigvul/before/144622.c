void WebContentsImpl::OnUpdateZoomLimits(int minimum_percent,
                                         int maximum_percent) {
  minimum_zoom_percent_ = minimum_percent;
  maximum_zoom_percent_ = maximum_percent;
}
