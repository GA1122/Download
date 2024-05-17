void DownloadItemImpl::Delegate::Detach() {
  DCHECK_LT(0, count_);
  --count_;
}
