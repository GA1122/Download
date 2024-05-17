void RecordDangerousDownloadDiscard(DownloadDiscardReason reason,
                                    DownloadDangerType danger_type,
                                    const base::FilePath& file_path) {
  switch (reason) {
    case DOWNLOAD_DISCARD_DUE_TO_USER_ACTION:
      UMA_HISTOGRAM_ENUMERATION("Download.UserDiscard", danger_type,
                                DOWNLOAD_DANGER_TYPE_MAX);
      if (danger_type == DOWNLOAD_DANGER_TYPE_DANGEROUS_FILE) {
        base::UmaHistogramSparse("Download.DangerousFile.UserDiscard",
                                 GetDangerousFileType(file_path));
      }
      break;
    case DOWNLOAD_DISCARD_DUE_TO_SHUTDOWN:
      UMA_HISTOGRAM_ENUMERATION("Download.Discard", danger_type,
                                DOWNLOAD_DANGER_TYPE_MAX);
      if (danger_type == DOWNLOAD_DANGER_TYPE_DANGEROUS_FILE) {
        base::UmaHistogramSparse("Download.DangerousFile.Discard",
                                 GetDangerousFileType(file_path));
      }
      break;
    default:
      NOTREACHED();
  }
}
