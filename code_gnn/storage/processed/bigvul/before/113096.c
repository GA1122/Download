void DownloadItemImpl::SetDbHandle(int64 handle) {
  db_handle_ = handle;

  bound_net_log_.AddEvent(
      net::NetLog::TYPE_DOWNLOAD_ITEM_IN_HISTORY,
      net::NetLog::Int64Callback("db_handle", db_handle_));
}
