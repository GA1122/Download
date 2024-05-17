void RecordArchivedArchiveFileExtensionType(
    const base::FilePath::StringType& extension) {
  UMA_HISTOGRAM_ENUMERATION(
      "SBClientDownload.ArchivedArchiveExtensions",
      download_protection_util::GetSBClientDownloadExtensionValueForUMA(
          base::FilePath(extension)),
      download_protection_util::kSBClientDownloadExtensionsMax);
}
