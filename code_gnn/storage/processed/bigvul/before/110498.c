void GLES2DecoderImpl::DoPushGroupMarkerEXT(
    GLsizei length, const GLchar* marker) {
  if (!marker) {
    marker = "";
  }
  debug_marker_manager_.PushGroup(
      length ? std::string(marker, length) : std::string(marker));
}
