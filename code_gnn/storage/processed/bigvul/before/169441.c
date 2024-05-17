int FileStream::Close(const CompletionCallback& callback) {
  context_->Close(callback);
  return ERR_IO_PENDING;
}
