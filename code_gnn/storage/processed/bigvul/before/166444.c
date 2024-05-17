void RecordMaliciousDownloadClassified(DownloadDangerType danger_type) {
  UMA_HISTOGRAM_ENUMERATION("Download.MaliciousDownloadClassified", danger_type,
                            DOWNLOAD_DANGER_TYPE_MAX);
}
