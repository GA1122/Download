void MaybeReportDeepScanningVerdict(Profile* profile,
                                    const GURL& url,
                                    const std::string& file_name,
                                    const std::string& download_digest_sha256,
                                    const std::string& mime_type,
                                    const std::string& trigger,
                                    const int64_t content_size,
                                    BinaryUploadService::Result result,
                                    DeepScanningClientResponse response) {
  if (result == BinaryUploadService::Result::FILE_TOO_LARGE) {
    extensions::SafeBrowsingPrivateEventRouterFactory::GetForProfile(profile)
        ->OnLargeUnscannedFileEvent(url, file_name, download_digest_sha256,
                                    mime_type, trigger, content_size);
  }

  if (result != BinaryUploadService::Result::SUCCESS)
    return;

  if (response.malware_scan_verdict().verdict() ==
          MalwareDeepScanningVerdict::UWS ||
      response.malware_scan_verdict().verdict() ==
          MalwareDeepScanningVerdict::MALWARE) {
    extensions::SafeBrowsingPrivateEventRouterFactory::GetForProfile(profile)
        ->OnDangerousDeepScanningResult(
            url, file_name, download_digest_sha256,
            MalwareVerdictToThreatType(
                response.malware_scan_verdict().verdict()),
            mime_type, trigger, content_size);
  }

  if (response.dlp_scan_verdict().status() == DlpDeepScanningVerdict::SUCCESS) {
    if (!response.dlp_scan_verdict().triggered_rules().empty()) {
      extensions::SafeBrowsingPrivateEventRouterFactory::GetForProfile(profile)
          ->OnSensitiveDataEvent(response.dlp_scan_verdict(), url, file_name,
                                 download_digest_sha256, mime_type, trigger,
                                 content_size);
    }
  }
}
