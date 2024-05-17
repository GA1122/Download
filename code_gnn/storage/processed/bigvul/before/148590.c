void WebContentsImpl::UpdateZoom(double level) {
  SendPageMessage(new PageMsg_SetZoomLevel(
      MSG_ROUTING_NONE,
      PageMsg_SetZoomLevel_Command::USE_CURRENT_TEMPORARY_MODE, level));
}
