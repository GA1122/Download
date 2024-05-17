bool TopSitesImpl::IsBlacklisted(const GURL& url) {
  DCHECK(thread_checker_.CalledOnValidThread());
  const base::DictionaryValue* blacklist =
      pref_service_->GetDictionary(kMostVisitedURLsBlacklist);
  return blacklist && blacklist->HasKey(GetURLHash(url));
}
