DownloadRequestLimiter::GetDownloadUiStatus(
    content::WebContents* web_contents) {
  TabDownloadState* state = GetDownloadState(web_contents, nullptr, false);
  return state ? state->download_ui_status() : DOWNLOAD_UI_DEFAULT;
}
