int FileStream::Seek(int64_t offset, const Int64CompletionCallback& callback) {
  if (!IsOpen())
    return ERR_UNEXPECTED;

  context_->Seek(offset, callback);
  return ERR_IO_PENDING;
}
