void ChromeDownloadManagerDelegate::MaybeSendDangerousDownloadOpenedReport(
    DownloadItem* download,
    bool show_download_in_folder) {
#if defined(FULL_SAFE_BROWSING)
  safe_browsing::DownloadProtectionService* service =
      GetDownloadProtectionService();
  if (service) {
    service->MaybeSendDangerousDownloadOpenedReport(download,
                                                    show_download_in_folder);
  }
#endif
}
