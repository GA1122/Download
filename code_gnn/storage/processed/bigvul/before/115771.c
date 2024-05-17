SafeBrowsingBlockingPage::SafeBrowsingBlockingPage(
    SafeBrowsingService* sb_service,
    TabContents* tab_contents,
    const UnsafeResourceList& unsafe_resources)
    : ChromeInterstitialPage(tab_contents,
                             IsMainPageLoadBlocked(unsafe_resources),
                             unsafe_resources[0].url),
      malware_details_proceed_delay_ms_(
          kMalwareDetailsProceedDelayMilliSeconds),
      sb_service_(sb_service),
      is_main_frame_load_blocked_(IsMainPageLoadBlocked(unsafe_resources)),
      unsafe_resources_(unsafe_resources) {
  RecordUserAction(SHOW);
  if (!is_main_frame_load_blocked_) {
    navigation_entry_index_to_remove_ =
        tab()->controller().last_committed_entry_index();
  } else {
    navigation_entry_index_to_remove_ = -1;
  }

  if (unsafe_resources.size() == 1 &&
      unsafe_resources[0].threat_type == SafeBrowsingService::URL_MALWARE &&
      malware_details_ == NULL &&
      CanShowMalwareDetailsOption()) {
    malware_details_ = MalwareDetails::NewMalwareDetails(
        sb_service_, tab(), unsafe_resources[0]);
  }
}
