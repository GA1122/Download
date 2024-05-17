bool ChromeDownloadManagerDelegate::IsDownloadReadyForCompletion(
    DownloadItem* item,
    const base::Closure& internal_complete_callback) {
#if defined(ENABLE_SAFE_BROWSING)
  SafeBrowsingState* state = static_cast<SafeBrowsingState*>(
      item->GetExternalData(&safe_browsing_id));
  if (!state) {
    DownloadProtectionService* service = GetDownloadProtectionService();
    if (service) {
      VLOG(2) << __FUNCTION__ << "() Start SB download check for download = "
              << item->DebugString(false);
      state = new SafeBrowsingState();
      state->set_callback(internal_complete_callback);
      item->SetExternalData(&safe_browsing_id, state);
      service->CheckClientDownload(
          DownloadProtectionService::DownloadInfo::FromDownloadItem(*item),
          base::Bind(
              &ChromeDownloadManagerDelegate::CheckClientDownloadDone,
              this,
              item->GetId()));
      return false;
    }
  } else if (!state->is_complete()) {
    state->set_callback(internal_complete_callback);
    return false;
  }
#endif

#if defined(OS_CHROMEOS)
  if (!gdata::GDataDownloadObserver::IsReadyToComplete(
        item, internal_complete_callback))
    return false;
#endif
  return true;
}
