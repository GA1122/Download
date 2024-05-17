void DriveFileStreamReader::OnGetFileContentByPathCompletion(
    const InitializeCompletionCallback& callback,
    FileError error) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));

  if (reader_proxy_) {
    reader_proxy_->OnCompleted(error);
  } else {
    if (error != FILE_ERROR_OK) {
      callback.Run(error, scoped_ptr<DriveEntryProto>());
    }
  }
}
