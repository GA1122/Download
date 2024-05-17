bool GLES2DecoderImpl::ProcessPendingQueries(bool did_finish) {
  if (query_manager_.get() == NULL) {
    return false;
  }
  if (!query_manager_->ProcessPendingQueries(did_finish)) {
    current_decoder_error_ = error::kOutOfBounds;
  }
  return query_manager_->HavePendingQueries();
}
