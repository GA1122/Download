DownloadConnectionSecurity CheckDownloadConnectionSecurity(
    const GURL& download_url,
    const std::vector<GURL>& url_chain) {
  DownloadConnectionSecurity state = DOWNLOAD_TARGET_OTHER;
  if (download_url.SchemeIsHTTPOrHTTPS()) {
    bool is_final_download_secure = download_url.SchemeIsCryptographic();
    bool is_redirect_chain_secure = true;
    if (url_chain.size() > std::size_t(1)) {
      for (std::size_t i = std::size_t(0); i < url_chain.size() - 1; i++) {
        if (!url_chain[i].SchemeIsCryptographic()) {
          is_redirect_chain_secure = false;
          break;
        }
      }
    }
    state = is_final_download_secure
                ? is_redirect_chain_secure ? DOWNLOAD_SECURE
                                           : DOWNLOAD_REDIRECT_INSECURE
                : is_redirect_chain_secure ? DOWNLOAD_TARGET_INSECURE
                                           : DOWNLOAD_REDIRECT_TARGET_INSECURE;
  } else if (download_url.SchemeIsBlob()) {
    state = DOWNLOAD_TARGET_BLOB;
  } else if (download_url.SchemeIs(url::kDataScheme)) {
    state = DOWNLOAD_TARGET_DATA;
  } else if (download_url.SchemeIsFile()) {
    state = DOWNLOAD_TARGET_FILE;
  } else if (download_url.SchemeIsFileSystem()) {
    state = DOWNLOAD_TARGET_FILESYSTEM;
  } else if (download_url.SchemeIs(url::kFtpScheme)) {
    state = DOWNLOAD_TARGET_FTP;
  }
  return state;
}
