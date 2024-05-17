void RecordDownloadMimeType(const std::string& mime_type_string) {
  DownloadContent download_content =
      DownloadContentFromMimeType(mime_type_string, true);
  UMA_HISTOGRAM_ENUMERATION("Download.Start.ContentType", download_content,
                            DownloadContent::MAX);
}
