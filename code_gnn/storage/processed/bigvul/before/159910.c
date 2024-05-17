int TopSitesImpl::num_results_to_request_from_history() const {
  DCHECK(thread_checker_.CalledOnValidThread());

  const base::DictionaryValue* blacklist =
      pref_service_->GetDictionary(kMostVisitedURLsBlacklist);
  return kNonForcedTopSitesNumber + (blacklist ? blacklist->size() : 0);
}
