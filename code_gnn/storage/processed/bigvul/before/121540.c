LocalReaderProxy::LocalReaderProxy(scoped_ptr<net::FileStream> file_stream)
    : file_stream_(file_stream.Pass()) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
  DCHECK(file_stream_);
}
