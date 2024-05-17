void BinaryUploadService::RecordRequestMetrics(
    Request* request,
    Result result,
    const DeepScanningClientResponse& response) {
  base::UmaHistogramEnumeration("SafeBrowsingBinaryUploadRequest.Result",
                                result);
  base::UmaHistogramCustomTimes("SafeBrowsingBinaryUploadRequest.Duration",
                                base::TimeTicks::Now() - start_times_[request],
                                base::TimeDelta::FromMilliseconds(1),
                                base::TimeDelta::FromMinutes(6), 50);

  if (response.has_malware_scan_verdict()) {
    base::UmaHistogramBoolean("SafeBrowsingBinaryUploadRequest.MalwareResult",
                              response.malware_scan_verdict().verdict() !=
                                  MalwareDeepScanningVerdict::CLEAN);
  }

  if (response.has_dlp_scan_verdict()) {
    base::UmaHistogramBoolean("SafeBrowsingBinaryUploadRequest.DlpResult",
                              response.dlp_scan_verdict().status() ==
                                  DlpDeepScanningVerdict::SUCCESS);
  }
}
