void RecordDownloadDeletion(base::Time completion_time,
                            const std::string& mime_type) {
  if (completion_time == base::Time())
    return;

  base::TimeDelta retention_time = base::Time::Now() - completion_time;
  int retention_hours = retention_time.InHours();

  DownloadContent type = DownloadContentFromMimeType(mime_type, false);
  if (type == DownloadContent::VIDEO) {
    UMA_HISTOGRAM_CUSTOM_COUNTS("Download.DeleteRetentionTime.Video",
                                retention_hours, 1, kMaxDeletionRetentionHours,
                                50);
  }
  if (type == DownloadContent::AUDIO) {
    UMA_HISTOGRAM_CUSTOM_COUNTS("Download.DeleteRetentionTime.Audio",
                                retention_hours, 1, kMaxDeletionRetentionHours,
                                50);
  }
}
