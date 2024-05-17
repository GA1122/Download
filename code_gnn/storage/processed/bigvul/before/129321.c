void GLES2DecoderImpl::DoTraceEndCHROMIUM() {
  if (gpu_tracer_->CurrentName().empty()) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION,
        "glTraceEndCHROMIUM", "no trace begin found");
    return;
  }
  TRACE_EVENT_COPY_ASYNC_END0("gpu", gpu_tracer_->CurrentName().c_str(), this);
  gpu_tracer_->End(kTraceCHROMIUM);
}
