void FileAPIMessageFilter::OnAppendBlobDataItem(
    const GURL& url, const BlobData::Item& item) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
  if (item.type() == BlobData::Item::TYPE_FILE &&
      !ChildProcessSecurityPolicyImpl::GetInstance()->CanReadFile(
          process_id_, item.path())) {
    OnRemoveBlob(url);
    return;
  }
  if (item.length() == 0) {
    BadMessageReceived();
    return;
  }
  blob_storage_context_->controller()->AppendBlobDataItem(url, item);
}
