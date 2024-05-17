void TestingAutomationProvider::GetPageType(
    int handle,
    bool* success,
    content::PageType* page_type) {
  if (tab_tracker_->ContainsHandle(handle)) {
    NavigationController* tab = tab_tracker_->GetResource(handle);
    NavigationEntry* entry = tab->GetActiveEntry();
    *page_type = entry->GetPageType();
    *success = true;
    if (*page_type == content::PAGE_TYPE_NORMAL &&
        tab->GetWebContents()->ShowingInterstitialPage())
      *page_type = content::PAGE_TYPE_INTERSTITIAL;
  } else {
    *success = false;
    *page_type = content::PAGE_TYPE_NORMAL;
  }
}
