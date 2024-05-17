int FileStream::Read(IOBuffer* buf,
                     int buf_len,
                     const CompletionCallback& callback) {
  if (!IsOpen())
    return ERR_UNEXPECTED;

  DCHECK_GT(buf_len, 0);

  return context_->Read(buf, buf_len, callback);
}
