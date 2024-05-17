void RecordDangerousDownloadAccept(DownloadDangerType danger_type,
                                   const base::FilePath& file_path) {
  UMA_HISTOGRAM_ENUMERATION("Download.DangerousDownloadValidated", danger_type,
                            DOWNLOAD_DANGER_TYPE_MAX);
  if (danger_type == DOWNLOAD_DANGER_TYPE_DANGEROUS_FILE) {
    base::UmaHistogramSparse(
        "Download.DangerousFile.DangerousDownloadValidated",
        GetDangerousFileType(file_path));
  }
}
