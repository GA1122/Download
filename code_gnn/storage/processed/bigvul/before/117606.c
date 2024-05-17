bool SessionCrashedInfoBarDelegate::Accept() {
  uint32 behavior = 0;
  Browser* browser = BrowserList::GetLastActiveWithProfile(profile_);
  if (browser && browser->tab_count() == 1
      && browser->GetWebContentsAt(0)->GetURL() ==
      GURL(chrome::kChromeUINewTabURL)) {
    behavior = SessionRestore::CLOBBER_CURRENT_TAB;
  }
  SessionRestore::RestoreSession(
      profile_, browser, behavior, std::vector<GURL>());
  return true;
}
