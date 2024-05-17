bool AwContents::ShouldDownloadFavicon(const GURL& icon_url) {
  return g_should_download_favicons;
}
