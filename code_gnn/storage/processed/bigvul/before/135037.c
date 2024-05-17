void AppCacheUpdateJob::URLFetcher::Start() {
  request_->set_first_party_for_cookies(job_->manifest_url_);
  if (fetch_type_ == MANIFEST_FETCH && job_->doing_full_update_check_)
    request_->SetLoadFlags(request_->load_flags() | net::LOAD_BYPASS_CACHE);
  else if (existing_response_headers_.get())
    AddConditionalHeaders(existing_response_headers_.get());
  request_->Start();
}
