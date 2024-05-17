void GuestViewBase::ContentsZoomChange(bool zoom_in) {
  ui_zoom::PageZoom::Zoom(
      embedder_web_contents(),
      zoom_in ? content::PAGE_ZOOM_IN : content::PAGE_ZOOM_OUT);
}
