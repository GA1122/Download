void GLES2DecoderImpl::DoFlush() {
  glFlush();
  ProcessPendingQueries(false);
}
