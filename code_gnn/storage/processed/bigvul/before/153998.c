void GLES2DecoderImpl::DoFlush() {
  api()->glFlushFn();
  ProcessPendingQueries(false);
}
