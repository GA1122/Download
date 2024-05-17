void DownloadManagerImpl::OpenDownload(DownloadItemImpl* download) {
  int num_unopened = 0;
  for (const auto& it : downloads_) {
    DownloadItemImpl* item = it.second.get();
    if ((item->GetState() == DownloadItem::COMPLETE) &&
        !item->GetOpened())
      ++num_unopened;
  }
  RecordOpensOutstanding(num_unopened);

  if (delegate_)
     delegate_->OpenDownload(download);
 }
