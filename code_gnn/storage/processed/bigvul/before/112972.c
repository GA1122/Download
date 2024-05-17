bool ChromeDownloadManagerDelegate::ShouldOpenWithWebIntents(
    const DownloadItem* item) {
  if (!item->GetWebContents() || !item->GetWebContents()->GetDelegate())
    return false;

  std::string mime_type = item->GetMimeType();
  if (mime_type == "application/rss+xml" ||
      mime_type == "application/atom+xml") {
    return true;
  }

#if defined(OS_CHROMEOS)
  if (mime_type == "application/msword" ||
      mime_type == "application/vnd.ms-powerpoint" ||
      mime_type == "application/vnd.ms-excel" ||
      mime_type == "application/vnd.openxmlformats-officedocument."
                   "wordprocessingml.document" ||
      mime_type == "application/vnd.openxmlformats-officedocument."
                   "presentationml.presentation" ||
      mime_type == "application/vnd.openxmlformats-officedocument."
                   "spreadsheetml.sheet") {
    return true;
  }
#endif   

  return false;
}
