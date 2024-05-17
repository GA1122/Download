error::Error GLES2DecoderPassthroughImpl::DoTraceEndCHROMIUM() {
  if (!gpu_tracer_->End(kTraceCHROMIUM)) {
    InsertError(GL_INVALID_OPERATION, "No trace to end");
    return error::kNoError;
  }
  debug_marker_manager_.PopGroup();
  return error::kNoError;
}
