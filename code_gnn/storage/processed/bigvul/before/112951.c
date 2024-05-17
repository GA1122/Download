void ChromeDownloadManagerDelegate::DisableSafeBrowsing(DownloadItem* item) {
#if defined(ENABLE_SAFE_BROWSING)
  SafeBrowsingState* state = static_cast<SafeBrowsingState*>(
      item->GetExternalData(&safe_browsing_id));
  DCHECK(!state);
  if (!state)
    state = new SafeBrowsingState();
  state->SetVerdict(DownloadProtectionService::SAFE);
  item->SetExternalData(&safe_browsing_id, state);
#endif
}
