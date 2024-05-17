void DriveFileStreamReader::InitializeAfterLocalFileOpen(
    const InitializeCompletionCallback& callback,
    scoped_ptr<DriveEntryProto> entry,
    scoped_ptr<net::FileStream> file_stream,
    int open_result) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));

  if (open_result != net::OK) {
    callback.Run(FILE_ERROR_FAILED, scoped_ptr<DriveEntryProto>());
    return;
  }

  reader_proxy_.reset(new internal::LocalReaderProxy(file_stream.Pass()));
  callback.Run(FILE_ERROR_OK, entry.Pass());
}
