void GuestViewBase::StopTrackingEmbedderZoomLevel() {
  if (!attached() || !ZoomPropagatesFromEmbedderToGuest())
    return;

  auto embedder_zoom_controller =
      ui_zoom::ZoomController::FromWebContents(owner_web_contents());
  if (!embedder_zoom_controller)
    return;
   embedder_zoom_controller->RemoveObserver(this);
 }
