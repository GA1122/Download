bool CheckClientDownloadRequest::IsCancelled() {
  return item_->GetState() == download::DownloadItem::CANCELLED;
}
