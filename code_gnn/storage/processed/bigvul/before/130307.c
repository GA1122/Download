void DataObjectImpl::StopDownloads() {
  for (StoredData::iterator iter = contents_.begin();
       iter != contents_.end(); ++iter) {
    if ((*iter)->downloader.get()) {
      (*iter)->downloader->Stop();
      (*iter)->downloader = 0;
    }
  }
}
