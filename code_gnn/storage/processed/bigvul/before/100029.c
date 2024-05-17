void WebPluginImpl::updateGeometry(
    const WebRect& window_rect, const WebRect& clip_rect,
    const WebVector<WebRect>& cutout_rects, bool is_visible) {
  WebPluginGeometry new_geometry;
  new_geometry.window = window_;
  new_geometry.window_rect = window_rect;
  new_geometry.clip_rect = clip_rect;
  new_geometry.visible = is_visible;
  new_geometry.rects_valid = true;
  for (size_t i = 0; i < cutout_rects.size(); ++i)
    new_geometry.cutout_rects.push_back(cutout_rects[i]);

  if (window_ &&
      page_delegate_ &&
      (first_geometry_update_ || !new_geometry.Equals(geometry_))) {
    page_delegate_->DidMovePlugin(new_geometry);
  }

  if (first_geometry_update_ ||
      new_geometry.window_rect != geometry_.window_rect ||
      new_geometry.clip_rect != geometry_.clip_rect) {
    delegate_->UpdateGeometry(new_geometry.window_rect, new_geometry.clip_rect);
  }

  if (first_geometry_update_) {
    if (!load_manually_ && plugin_url_.is_valid()) {
      MessageLoop::current()->PostDelayedTask(
          FROM_HERE, method_factory_.NewRunnableMethod(
              &WebPluginImpl::OnDownloadPluginSrcUrl), 0);
    }
  }

  first_geometry_update_ = false;
  geometry_ = new_geometry;
}
