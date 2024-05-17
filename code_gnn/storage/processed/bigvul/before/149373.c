void DeepScanningClientResponseToDownloadCheckResult(
    const DeepScanningClientResponse& response,
    DownloadCheckResult* download_result,
    DownloadCheckResultReason* download_reason) {
  if (response.has_malware_scan_verdict() &&
      response.malware_scan_verdict().verdict() ==
          MalwareDeepScanningVerdict::MALWARE) {
    *download_result = DownloadCheckResult::DANGEROUS;
    *download_reason = DownloadCheckResultReason::REASON_DOWNLOAD_DANGEROUS;
    return;
  }

  if (response.has_malware_scan_verdict() &&
      response.malware_scan_verdict().verdict() ==
          MalwareDeepScanningVerdict::UWS) {
    *download_result = DownloadCheckResult::POTENTIALLY_UNWANTED;
    *download_reason =
        DownloadCheckResultReason::REASON_DOWNLOAD_POTENTIALLY_UNWANTED;
    return;
  }

  if (response.has_dlp_scan_verdict()) {
    bool should_dlp_block = std::any_of(
        response.dlp_scan_verdict().triggered_rules().begin(),
        response.dlp_scan_verdict().triggered_rules().end(),
        [](const DlpDeepScanningVerdict::TriggeredRule& rule) {
          return rule.action() == DlpDeepScanningVerdict::TriggeredRule::BLOCK;
        });
    if (should_dlp_block) {
      *download_result = DownloadCheckResult::SENSITIVE_CONTENT_BLOCK;
      *download_reason =
          DownloadCheckResultReason::REASON_SENSITIVE_CONTENT_BLOCK;
      return;
    }

    bool should_dlp_warn = std::any_of(
        response.dlp_scan_verdict().triggered_rules().begin(),
        response.dlp_scan_verdict().triggered_rules().end(),
        [](const DlpDeepScanningVerdict::TriggeredRule& rule) {
          return rule.action() == DlpDeepScanningVerdict::TriggeredRule::WARN;
        });
    if (should_dlp_warn) {
      *download_result = DownloadCheckResult::SENSITIVE_CONTENT_WARNING;
      *download_reason =
          DownloadCheckResultReason::REASON_SENSITIVE_CONTENT_WARNING;
      return;
    }
  }

  *download_result = DownloadCheckResult::DEEP_SCANNED_SAFE;
  *download_reason = DownloadCheckResultReason::REASON_DEEP_SCANNED_SAFE;
}
