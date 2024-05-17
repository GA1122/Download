void RecordDownloadConnectionSecurity(const GURL& download_url,
                                      const std::vector<GURL>& url_chain) {
  UMA_HISTOGRAM_ENUMERATION(
      "Download.TargetConnectionSecurity",
      CheckDownloadConnectionSecurity(download_url, url_chain),
      DOWNLOAD_CONNECTION_SECURITY_MAX);
}
