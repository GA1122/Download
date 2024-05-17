void TranslateManager::RequestTranslateScript() {
  if (translate_script_request_pending_)
    return;

  translate_script_request_pending_ = true;
  URLFetcher* fetcher = URLFetcher::Create(0, GURL(kTranslateScriptURL),
                                           URLFetcher::GET, this);
  fetcher->set_request_context(Profile::GetDefaultRequestContext());
  fetcher->set_extra_request_headers(kTranslateScriptHeader);
  fetcher->Start();
}
