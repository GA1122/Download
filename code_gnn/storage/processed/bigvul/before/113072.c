bool DownloadItemImpl::IsDangerous() const {
#ifdef OS_WIN
  return (danger_type_ == content::DOWNLOAD_DANGER_TYPE_DANGEROUS_FILE ||
          danger_type_ == content::DOWNLOAD_DANGER_TYPE_DANGEROUS_URL ||
          danger_type_ == content::DOWNLOAD_DANGER_TYPE_DANGEROUS_CONTENT ||
          danger_type_ == content::DOWNLOAD_DANGER_TYPE_UNCOMMON_CONTENT);
#else
  return (danger_type_ == content::DOWNLOAD_DANGER_TYPE_DANGEROUS_FILE ||
          danger_type_ == content::DOWNLOAD_DANGER_TYPE_DANGEROUS_URL);
#endif
}
