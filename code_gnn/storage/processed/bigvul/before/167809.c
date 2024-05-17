void DownloadRequestLimiter::CanDownloadImpl(
    content::WebContents* originating_contents,
    const std::string& request_method,
    const Callback& callback) {
  DCHECK(originating_contents);

  TabDownloadState* state =
      GetDownloadState(originating_contents, originating_contents, true);
  state->set_download_seen();
  bool ret = true;

  switch (state->download_status()) {
    case ALLOW_ALL_DOWNLOADS:
      if (state->download_count() &&
          !(state->download_count() %
            DownloadRequestLimiter::kMaxDownloadsAtOnce)) {
        state->SetDownloadStatusAndNotify(PROMPT_BEFORE_DOWNLOAD);
      } else {
        state->SetDownloadStatusAndNotify(ALLOW_ALL_DOWNLOADS);
      }
      callback.Run(true);
      state->increment_download_count();
      break;

    case ALLOW_ONE_DOWNLOAD:
      state->SetDownloadStatusAndNotify(PROMPT_BEFORE_DOWNLOAD);
      callback.Run(true);
      state->increment_download_count();
      break;

    case DOWNLOADS_NOT_ALLOWED:
      state->SetDownloadStatusAndNotify(DOWNLOADS_NOT_ALLOWED);
      ret = false;
      callback.Run(false);
      break;

    case PROMPT_BEFORE_DOWNLOAD: {
      HostContentSettingsMap* content_settings =
          GetContentSettings(originating_contents);
      ContentSetting setting = CONTENT_SETTING_ASK;
      if (content_settings) {
        setting = content_settings->GetContentSetting(
            originating_contents->GetURL(), originating_contents->GetURL(),
            CONTENT_SETTINGS_TYPE_AUTOMATIC_DOWNLOADS, std::string());
      }
      switch (setting) {
        case CONTENT_SETTING_ALLOW: {
          state->SetDownloadStatusAndNotify(ALLOW_ALL_DOWNLOADS);
          callback.Run(true);
          state->increment_download_count();
          break;
        }
        case CONTENT_SETTING_BLOCK: {
          state->SetDownloadStatusAndNotify(DOWNLOADS_NOT_ALLOWED);
          ret = false;
          callback.Run(false);
          break;
        }
        case CONTENT_SETTING_DEFAULT:
        case CONTENT_SETTING_ASK:
          state->PromptUserForDownload(callback);
          state->increment_download_count();
          break;
        case CONTENT_SETTING_SESSION_ONLY:
        case CONTENT_SETTING_NUM_SETTINGS:
        default:
          NOTREACHED();
          return;
      }
      break;
    }

    default:
      NOTREACHED();
  }

  if (!on_can_download_decided_callback_.is_null())
    on_can_download_decided_callback_.Run(ret);
}
