  void UpdateContentSettings(WebContents* web_contents,
                             ContentSetting setting) {
    download_request_limiter_->GetDownloadState(web_contents, nullptr, true);
    SetHostContentSetting(web_contents, setting);
  }
