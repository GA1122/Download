bool ChromeDownloadManagerDelegate::ShouldBlockFile(
    content::DownloadDangerType danger_type) const {
  DownloadPrefs::DownloadRestriction download_restriction =
      download_prefs_->download_restriction();

  switch (download_restriction) {
    case (DownloadPrefs::DownloadRestriction::NONE):
      return false;

    case (DownloadPrefs::DownloadRestriction::POTENTIALLY_DANGEROUS_FILES):
      return danger_type != content::DOWNLOAD_DANGER_TYPE_NOT_DANGEROUS;

    case (DownloadPrefs::DownloadRestriction::DANGEROUS_FILES): {
      return (danger_type == content::DOWNLOAD_DANGER_TYPE_DANGEROUS_CONTENT ||
              danger_type == content::DOWNLOAD_DANGER_TYPE_DANGEROUS_FILE ||
              danger_type == content::DOWNLOAD_DANGER_TYPE_DANGEROUS_URL);
    }

    case (DownloadPrefs::DownloadRestriction::ALL_FILES):
      return true;

    default:
      LOG(ERROR) << "Invalid download restruction value: "
                 << static_cast<int>(download_restriction);
  }
  return false;
}
