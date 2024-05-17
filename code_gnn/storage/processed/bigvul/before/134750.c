void GuestViewBase::SetGuestZoomLevelToMatchEmbedder() {
  auto embedder_zoom_controller =
      ui_zoom::ZoomController::FromWebContents(owner_web_contents());
  if (!embedder_zoom_controller)
    return;

  ui_zoom::ZoomController::FromWebContents(web_contents())
      ->SetZoomLevel(embedder_zoom_controller->GetZoomLevel());
}
