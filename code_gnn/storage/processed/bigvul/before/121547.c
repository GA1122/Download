int DriveFileStreamReader::Read(net::IOBuffer* buffer, int buffer_length,
                                const net::CompletionCallback& callback) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
  DCHECK(reader_proxy_);
  DCHECK(buffer);
  DCHECK(!callback.is_null());
  return reader_proxy_->Read(buffer, buffer_length, callback);
}
