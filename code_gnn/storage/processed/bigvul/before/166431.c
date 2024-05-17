void RecordDownloadCount(DownloadCountTypes type) {
  UMA_HISTOGRAM_ENUMERATION("Download.Counts", type,
                            DOWNLOAD_COUNT_TYPES_LAST_ENTRY);
}
