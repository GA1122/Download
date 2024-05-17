void GLES2DecoderImpl::DoPushGroupMarkerEXT(
    GLsizei length, const GLchar* marker) {
  if (!marker) {
    marker = "";
  }
  std::string name = length ? std::string(marker, length) : std::string(marker);
  debug_marker_manager_.PushGroup(name);
  gpu_tracer_->Begin(name, kTraceGroupMarker);
}
