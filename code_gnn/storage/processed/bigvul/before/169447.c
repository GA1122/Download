int FileStream::Write(IOBuffer* buf,
                      int buf_len,
                      const CompletionCallback& callback) {
  if (!IsOpen())
    return ERR_UNEXPECTED;

  DCHECK_GE(buf_len, 0);
  return context_->Write(buf, buf_len, callback);
}
