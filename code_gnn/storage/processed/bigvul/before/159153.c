void DownloadItemImpl::Init(bool active,
                            DownloadItem::DownloadType download_type) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  std::string file_name;
  if (download_type == TYPE_HISTORY_IMPORT) {
    file_name = GetTargetFilePath().AsUTF8Unsafe();
  } else {
    file_name = GetForcedFilePath().AsUTF8Unsafe();
    if (file_name.empty())
      file_name = GetSuggestedFilename();
    if (file_name.empty())
      file_name = GetURL().ExtractFileName();
  }

  auto active_data = std::make_unique<DownloadItemActivatedData>(
      download_type, GetId(), GetOriginalUrl().spec(), GetURL().spec(),
      file_name, GetDangerType(), GetReceivedBytes(), HasUserGesture());

  if (active) {
    TRACE_EVENT_ASYNC_BEGIN1("download", "DownloadItemActive", download_id_,
                             "download_item", std::move(active_data));
  } else {
    TRACE_EVENT_INSTANT1("download", "DownloadItemActive",
                         TRACE_EVENT_SCOPE_THREAD, "download_item",
                         std::move(active_data));
  }

  DVLOG(20) << __func__ << "() " << DebugString(true);
}
