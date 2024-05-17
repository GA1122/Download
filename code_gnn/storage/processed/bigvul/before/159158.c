bool DownloadItemImpl::IsDangerous() const {
  return (danger_type_ == DOWNLOAD_DANGER_TYPE_DANGEROUS_FILE ||
          danger_type_ == DOWNLOAD_DANGER_TYPE_DANGEROUS_URL ||
          danger_type_ == DOWNLOAD_DANGER_TYPE_DANGEROUS_CONTENT ||
          danger_type_ == DOWNLOAD_DANGER_TYPE_UNCOMMON_CONTENT ||
          danger_type_ == DOWNLOAD_DANGER_TYPE_DANGEROUS_HOST ||
          danger_type_ == DOWNLOAD_DANGER_TYPE_POTENTIALLY_UNWANTED);
}
