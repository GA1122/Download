void ChromeContentBrowserClient::ClearCache(RenderViewHost* rvh) {
  Profile* profile = rvh->site_instance()->GetProcess()->profile();
  BrowsingDataRemover* remover = new BrowsingDataRemover(profile,
      BrowsingDataRemover::EVERYTHING,
      base::Time());
  remover->Remove(BrowsingDataRemover::REMOVE_CACHE);
}
