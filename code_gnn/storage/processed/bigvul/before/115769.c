void SafeBrowsingBlockingPage::Proceed() {
  RecordUserAction(PROCEED);
  FinishMalwareDetails(malware_details_proceed_delay_ms_);

  NotifySafeBrowsingService(sb_service_, unsafe_resources_, true);

  UnsafeResourceMap* unsafe_resource_map = GetUnsafeResourcesMap();
  UnsafeResourceMap::iterator iter = unsafe_resource_map->find(tab());
  SafeBrowsingBlockingPage* blocking_page = NULL;
  if (iter != unsafe_resource_map->end() && !iter->second.empty()) {
    blocking_page = factory_->CreateSafeBrowsingPage(sb_service_, tab(),
                                                     iter->second);
    unsafe_resource_map->erase(iter);
  }

  InterstitialPage::Proceed();

  if (blocking_page)
    blocking_page->Show();
}
