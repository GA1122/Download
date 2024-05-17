  void DownloadItemImpl::UpdateProgress(int64 bytes_so_far,
//                                       int64 bytes_per_sec,
                                        const std::string& hash_state) {
  hash_state_ = hash_state;
//   DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  
//   if (!IsInProgress()) {
//      
//      
//      
//      
//      
//      
//      
//      
//      
//     return;
//   }
//   bytes_per_sec_ = bytes_per_sec;
//   hash_state_ = hash_state;
    received_bytes_ = bytes_so_far;
  
   if (received_bytes_ > total_bytes_)
     total_bytes_ = 0;
 
   if (bound_net_log_.IsLoggingAllEvents()) {
     bound_net_log_.AddEvent(
          net::NetLog::TYPE_DOWNLOAD_ITEM_UPDATED,
          net::NetLog::Int64Callback("bytes_so_far", received_bytes_));
    }
}