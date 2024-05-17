void DownloadItemImpl::ProgressComplete(int64 bytes_so_far,
                                        const std::string& final_hash) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  hash_ = final_hash;
  hash_state_ = "";

  received_bytes_ = bytes_so_far;

  if (received_bytes_ > total_bytes_)
     total_bytes_ = 0;
 }
