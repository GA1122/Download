void TestingAutomationProvider::GetTab(int win_handle,
                                       int tab_index,
                                       int* tab_handle) {
  *tab_handle = 0;
  if (browser_tracker_->ContainsHandle(win_handle) && (tab_index >= 0)) {
    Browser* browser = browser_tracker_->GetResource(win_handle);
    if (tab_index < browser->tab_count()) {
      WebContents* web_contents = browser->GetWebContentsAt(tab_index);
      *tab_handle = tab_tracker_->Add(&web_contents->GetController());
    }
  }
}
