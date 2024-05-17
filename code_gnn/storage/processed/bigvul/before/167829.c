DownloadRequestLimiter::TabDownloadState::TabDownloadState()
    : web_contents_(nullptr),
      host_(nullptr),
      status_(DownloadRequestLimiter::ALLOW_ONE_DOWNLOAD),
      ui_status_(DownloadRequestLimiter::DOWNLOAD_UI_DEFAULT),
      download_count_(0),
      download_seen_(false),
      observer_(this),
      factory_(this) {}
