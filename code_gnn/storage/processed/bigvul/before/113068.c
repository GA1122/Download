void DownloadItemImpl::Init(bool active,
                            download_net_logs::DownloadType download_type) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  if (active)
    download_stats::RecordDownloadCount(download_stats::START_COUNT);

  if (target_path_.empty())
    target_path_ = current_path_;
  std::string file_name;
  if (download_type == download_net_logs::SRC_HISTORY_IMPORT) {
    file_name = target_path_.AsUTF8Unsafe();
  } else {
    file_name = forced_file_path_.AsUTF8Unsafe();
    if (file_name.empty())
      file_name = suggested_filename_;
    if (file_name.empty())
      file_name = GetURL().ExtractFileName();
  }

  bound_net_log_.BeginEvent(
      net::NetLog::TYPE_DOWNLOAD_ITEM_ACTIVE,
      base::Bind(&download_net_logs::ItemActivatedCallback,
                 this, download_type, &file_name));

  if (!active) {
    bound_net_log_.AddEvent(
        net::NetLog::TYPE_DOWNLOAD_ITEM_IN_HISTORY,
        net::NetLog::Int64Callback("db_handle", db_handle_));

    bound_net_log_.EndEvent(net::NetLog::TYPE_DOWNLOAD_ITEM_ACTIVE);
  }

  VLOG(20) << __FUNCTION__ << "() " << DebugString(true);
}
