DocumentModuleScriptFetcher::DocumentModuleScriptFetcher(
    ResourceFetcher* fetcher)
    : fetcher_(fetcher) {
  DCHECK(fetcher_);
}
