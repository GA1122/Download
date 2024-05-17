void ChromeContentBrowserClient::ClearCookies(RenderViewHost* rvh) {
  Profile* profile = rvh->site_instance()->GetProcess()->profile();
  BrowsingDataRemover* remover = new BrowsingDataRemover(profile,
      BrowsingDataRemover::EVERYTHING,
      base::Time());
  int remove_mask = BrowsingDataRemover::REMOVE_COOKIES;
  remover->Remove(remove_mask);
}
