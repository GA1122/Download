void RenderViewImpl::ZoomFactorHelper(PageZoom zoom,
                                      int zoom_center_x,
                                      int zoom_center_y,
                                      float scaling_increment) {
  if (!webview())   
    return;

  double old_page_scale_factor = webview()->pageScaleFactor();
  double page_scale_factor;
  if (zoom == PAGE_ZOOM_RESET) {
    page_scale_factor = 1.0;
  } else {
    page_scale_factor = old_page_scale_factor +
        (zoom > 0 ? scaling_increment : -scaling_increment);
  }
  if (page_scale_factor > 0) {
    webview()->setPageScaleFactor(page_scale_factor,
                                  WebPoint(zoom_center_x, zoom_center_y));
  }
}
