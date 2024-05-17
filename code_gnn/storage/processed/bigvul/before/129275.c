void GLES2DecoderImpl::DoFinish() {
  glFinish();
  ProcessPendingReadPixels();
  ProcessPendingQueries();
}
