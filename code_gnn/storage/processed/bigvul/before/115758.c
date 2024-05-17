bool SafeBrowsingBlockingPage::CanShowMalwareDetailsOption() {
  return (!tab()->browser_context()->IsOffTheRecord() &&
          tab()->GetURL().SchemeIs(chrome::kHttpScheme));
}
