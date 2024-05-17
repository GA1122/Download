void GLES2DecoderImpl::DoTraceEndCHROMIUM() {
  if (!gpu_tracer_->End(kTraceCHROMIUM)) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION,
                       "glTraceEndCHROMIUM", "no trace begin found");
    return;
  }
}