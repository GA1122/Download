bool UrlmonUrlRequest::Read(int bytes_to_read) {
  DCHECK_EQ(thread_, base::PlatformThread::CurrentId());
  DCHECK_GE(bytes_to_read, 0);
  DCHECK_EQ(0, calling_delegate_);
  DVLOG(1) << __FUNCTION__ << me();

  is_expecting_download_ = false;

  DCHECK_EQ(0u, pending_read_size_);
  if (pending_read_size_ != 0)
    return false;

  DCHECK((status_.get_state() != Status::DONE) == (binding_ != NULL));
  if (status_.get_state() == Status::ABORTING)
    return true;

  size_t bytes_copied = 0;
  if ((bytes_copied = SendDataToDelegate(bytes_to_read))) {
    DVLOG(1) << __FUNCTION__ << me() << " bytes read: " << bytes_copied;
    return true;
  }

  if (status_.get_state() == Status::WORKING) {
    DVLOG(1) << __FUNCTION__ << me() << " pending: " << bytes_to_read;
    pending_read_size_ = bytes_to_read;
  } else {
    DVLOG(1) << __FUNCTION__ << me() << " Response finished.";
    NotifyDelegateAndDie();
  }

  return true;
}
