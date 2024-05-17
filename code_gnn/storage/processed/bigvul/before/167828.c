DownloadRequestLimiter::TabDownloadState::TabDownloadState(
    DownloadRequestLimiter* host,
    content::WebContents* contents,
    content::WebContents* originating_web_contents)
    : content::WebContentsObserver(contents),
      web_contents_(contents),
      host_(host),
      status_(DownloadRequestLimiter::ALLOW_ONE_DOWNLOAD),
      ui_status_(DownloadRequestLimiter::DOWNLOAD_UI_DEFAULT),
      download_count_(0),
      download_seen_(false),
      observer_(this),
      factory_(this) {
  observer_.Add(GetContentSettings(contents));
  NavigationEntry* last_entry =
      originating_web_contents
          ? originating_web_contents->GetController().GetLastCommittedEntry()
          : contents->GetController().GetLastCommittedEntry();
  if (last_entry)
    initial_page_host_ = last_entry->GetURL().host();
}
