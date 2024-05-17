bool GLES2DecoderImpl::ProcessPendingQueries() {
  if (query_manager_.get() == NULL) {
    return false;
  }
  if (!query_manager_->ProcessPendingQueries()) {
    current_decoder_error_ = error::kOutOfBounds;
  }
  return query_manager_->HavePendingQueries();
}
