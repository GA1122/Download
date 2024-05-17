void RecordFileExtensionType(const base::FilePath& file) {
  UMA_HISTOGRAM_ENUMERATION(
      "SBClientDownload.DownloadExtensions",
      download_protection_util::GetSBClientDownloadExtensionValueForUMA(file),
      download_protection_util::kSBClientDownloadExtensionsMax);
}
