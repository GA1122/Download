  void AddPageToHistory(const GURL& url,
                        const base::string16& title,
                        const history::RedirectList& redirects,
                        base::Time time) {
    history_service()->AddPage(
        url, time, reinterpret_cast<ContextID>(1), 0, GURL(),
        redirects, ui::PAGE_TRANSITION_TYPED, history::SOURCE_BROWSED,
        false);
    history_service()->SetPageTitle(url, title);
  }
