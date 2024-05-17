error::Error GLES2DecoderPassthroughImpl::DoTraceBeginCHROMIUM(
    const char* category_name,
    const char* trace_name) {
  if (!gpu_tracer_->Begin(category_name, trace_name, kTraceCHROMIUM)) {
    InsertError(GL_INVALID_OPERATION, "Failed to create begin trace");
    return error::kNoError;
  }
  debug_marker_manager_.PushGroup(trace_name);
  return error::kNoError;
}
