void OpenEditURLUIThread(Profile* profile, const GURL* edit_url) {
  Browser* browser = browser::FindLastActiveWithProfile(profile);
  if (browser) {
    browser->OpenURL(content::OpenURLParams(*edit_url, content::Referrer(),
        CURRENT_TAB, content::PAGE_TRANSITION_TYPED, false));
  }
}
