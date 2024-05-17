  void AddPageToHistory(const GURL& url, const base::string16& title) {
    RedirectList redirects;
    redirects.push_back(url);
    history_service()->AddPage(
        url, base::Time::Now(), reinterpret_cast<ContextID>(1), 0, GURL(),
        redirects, ui::PAGE_TRANSITION_TYPED, history::SOURCE_BROWSED, false);
    history_service()->SetPageTitle(url, title);
  }
