int LocalReaderProxy::Read(net::IOBuffer* buffer, int buffer_length,
                           const net::CompletionCallback& callback) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
  DCHECK(file_stream_);
  return file_stream_->Read(buffer, buffer_length, callback);
}
