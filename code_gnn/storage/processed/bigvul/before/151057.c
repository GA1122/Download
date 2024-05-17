void DevToolsWindow::ContentsZoomChange(bool zoom_in) {
  DCHECK(is_docked_);
  zoom::PageZoom::Zoom(main_web_contents_, zoom_in ? content::PAGE_ZOOM_IN
                                                   : content::PAGE_ZOOM_OUT);
}
