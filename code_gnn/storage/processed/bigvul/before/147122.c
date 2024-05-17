DocumentLoader::DocumentLoader(LocalFrame* frame,
                               const ResourceRequest& req,
                               const SubstituteData& substitute_data,
                               ClientRedirectPolicy client_redirect_policy)
    : frame_(frame),
      fetcher_(FrameFetchContext::CreateFetcherFromDocumentLoader(this)),
      original_request_(req),
      substitute_data_(substitute_data),
      request_(req),
      load_type_(kFrameLoadTypeStandard),
      is_client_redirect_(client_redirect_policy ==
                          ClientRedirectPolicy::kClientRedirect),
      replaces_current_history_item_(false),
      data_received_(false),
      navigation_type_(kNavigationTypeOther),
      document_load_timing_(*this),
      time_of_last_data_received_(0.0),
      application_cache_host_(ApplicationCacheHost::Create(this)),
      was_blocked_after_csp_(false),
      state_(kNotStarted),
      in_data_received_(false),
      data_buffer_(SharedBuffer::Create()) {
  DCHECK(frame_);

  if (is_client_redirect_)
    AppendRedirect(frame_->GetDocument()->Url());
}
