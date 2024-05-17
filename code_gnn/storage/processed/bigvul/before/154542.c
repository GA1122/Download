GLES2DecoderPassthroughImpl::PendingQuery::~PendingQuery() {
  RunCallbacks(std::move(callbacks));
}
