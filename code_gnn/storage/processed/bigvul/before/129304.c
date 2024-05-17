void GLES2DecoderImpl::DoPopGroupMarkerEXT(void) {
  debug_marker_manager_.PopGroup();
  gpu_tracer_->End(kTraceGroupMarker);
}
