bool DriveFileStreamReader::IsInitialized() const {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
  return reader_proxy_.get() != NULL;
}
