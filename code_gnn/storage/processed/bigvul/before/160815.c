void RenderViewImpl::OnSetZoomLevel(
    PageMsg_SetZoomLevel_Command command,
    double zoom_level) {
  switch (command) {
    case PageMsg_SetZoomLevel_Command::CLEAR_TEMPORARY:
      uses_temporary_zoom_level_ = false;
      break;
    case PageMsg_SetZoomLevel_Command::SET_TEMPORARY:
      uses_temporary_zoom_level_ = true;
      break;
    case PageMsg_SetZoomLevel_Command::USE_CURRENT_TEMPORARY_MODE:
      if (uses_temporary_zoom_level_)
        return;
      break;
    default:
      NOTIMPLEMENTED();
  }
  webview()->HidePopups();
  SetZoomLevel(zoom_level);
}
