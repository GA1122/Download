void GuestViewBase::OnZoomChanged(
    const ui_zoom::ZoomController::ZoomChangedEventData& data) {
  if (data.web_contents == embedder_web_contents()) {
    auto guest_zoom_controller =
        ui_zoom::ZoomController::FromWebContents(web_contents());
    if (content::ZoomValuesEqual(data.new_zoom_level,
                                 guest_zoom_controller->GetZoomLevel())) {
      return;
    }
    guest_zoom_controller->SetZoomLevel(data.new_zoom_level);

    EmbedderZoomChanged(data.old_zoom_level, data.new_zoom_level);
    return;
  }

  if (data.web_contents == web_contents()) {
    GuestZoomChanged(data.old_zoom_level, data.new_zoom_level);
  }
}
