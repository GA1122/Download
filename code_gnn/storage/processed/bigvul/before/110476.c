void GLES2DecoderImpl::DoFinish() {
  glFinish();
  ProcessPendingQueries();
}
