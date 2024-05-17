void TestingAutomationProvider::HideInterstitialPage(int tab_handle,
                                                     bool* success) {
  *success = false;
  WebContents* web_contents = GetWebContentsForHandle(tab_handle, NULL);
  if (web_contents && web_contents->GetInterstitialPage()) {
    web_contents->GetInterstitialPage()->DontProceed();
    *success = true;
  }
}
