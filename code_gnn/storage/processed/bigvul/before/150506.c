void CheckClientDownloadRequest::MaybeUploadBinary(DownloadCheckResult result,
                                                   const std::string& token) {
  if (ShouldUploadBinary(result)) {
    content::BrowserContext* browser_context = GetBrowserContext();
    if (browser_context) {
      Profile* profile = Profile::FromBrowserContext(browser_context);
      auto request = std::make_unique<DownloadItemRequest>(
          item_, base::BindOnce(&MaybeReportDownloadDeepScanningVerdict,
                                profile, item_->GetURL(),
                                item_->GetTargetFilePath().AsUTF8Unsafe(),
                                item_->GetHash()));

      DlpDeepScanningClientRequest dlp_request;
      dlp_request.set_content_source(
          DlpDeepScanningClientRequest::FILE_DOWNLOAD);
      request->set_request_dlp_scan(std::move(dlp_request));

      MalwareDeepScanningClientRequest malware_request;
      malware_request.set_population(
          MalwareDeepScanningClientRequest::POPULATION_ENTERPRISE);
      malware_request.set_download_token(token);
      request->set_request_malware_scan(std::move(malware_request));

      request->set_dm_token(
          policy::BrowserDMTokenStorage::Get()->RetrieveDMToken());

      service()->UploadForDeepScanning(profile, std::move(request));
    }
  }
}
