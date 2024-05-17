DocumentLoader::DocumentLoader(
    LocalFrame* frame,
    const ResourceRequest& req,
    const SubstituteData& substitute_data,
    ClientRedirectPolicy client_redirect_policy,
    const base::UnguessableToken& devtools_navigation_token)
    : frame_(frame),
      fetcher_(FrameFetchContext::CreateFetcherFromDocumentLoader(this)),
      original_request_(req),
      substitute_data_(substitute_data),
      request_(req),
      load_type_(WebFrameLoadType::kStandard),
      is_client_redirect_(client_redirect_policy ==
                          ClientRedirectPolicy::kClientRedirect),
      replaces_current_history_item_(false),
      data_received_(false),
      navigation_type_(kWebNavigationTypeOther),
      document_load_timing_(*this),
      application_cache_host_(ApplicationCacheHost::Create(this)),
      was_blocked_after_csp_(false),
      state_(kNotStarted),
      committed_data_buffer_(nullptr),
      in_data_received_(false),
      data_buffer_(SharedBuffer::Create()),
      devtools_navigation_token_(devtools_navigation_token),
      had_sticky_activation_(false),
      had_transient_activation_(false),
      use_counter_(frame_->GetChromeClient().IsSVGImageChromeClient()
                       ? UseCounter::kSVGImageContext
                       : UseCounter::kDefaultContext) {
  DCHECK(frame_);

  if (is_client_redirect_)
    AppendRedirect(frame_->GetDocument()->Url());
}
