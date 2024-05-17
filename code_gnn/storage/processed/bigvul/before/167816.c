DownloadRequestLimiter::GetDownloadStatus(content::WebContents* web_contents) {
  TabDownloadState* state = GetDownloadState(web_contents, nullptr, false);
  return state ? state->download_status() : ALLOW_ONE_DOWNLOAD;
}
