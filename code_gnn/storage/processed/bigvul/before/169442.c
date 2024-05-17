int FileStream::Flush(const CompletionCallback& callback) {
  if (!IsOpen())
    return ERR_UNEXPECTED;

  context_->Flush(callback);
  return ERR_IO_PENDING;
}
