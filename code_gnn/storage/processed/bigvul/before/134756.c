void GuestViewBase::StartTrackingEmbedderZoomLevel() {
  if (!ZoomPropagatesFromEmbedderToGuest())
    return;

  auto embedder_zoom_controller =
      ui_zoom::ZoomController::FromWebContents(owner_web_contents());
  if (!embedder_zoom_controller)
    return;
  embedder_zoom_controller->AddObserver(this);

  SetGuestZoomLevelToMatchEmbedder();
}
