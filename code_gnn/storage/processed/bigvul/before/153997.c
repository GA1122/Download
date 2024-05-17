void GLES2DecoderImpl::DoFinish() {
  api()->glFinishFn();
  ProcessPendingReadPixels(true);
  ProcessPendingQueries(true);
}
