void RecordDownloadMimeTypeForNormalProfile(
    const std::string& mime_type_string) {
  UMA_HISTOGRAM_ENUMERATION(
      "Download.Start.ContentType.NormalProfile",
      DownloadContentFromMimeType(mime_type_string, false),
      DownloadContent::MAX);
}
