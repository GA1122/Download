void GLES2Implementation::PopGroupMarkerEXT() {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glPopGroupMarkerEXT()");
  helper_->PopGroupMarkerEXT();
  debug_marker_manager_.PopGroup();
}
