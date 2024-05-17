void WebContentsImpl::SetTemporaryZoomLevel(double level,
                                            bool temporary_zoom_enabled) {
  SendPageMessage(new PageMsg_SetZoomLevel(
      MSG_ROUTING_NONE,
      temporary_zoom_enabled ? PageMsg_SetZoomLevel_Command::SET_TEMPORARY
                             : PageMsg_SetZoomLevel_Command::CLEAR_TEMPORARY,
      level));
}
