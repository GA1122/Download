DocumentLoader::DocumentLoader(
    LocalFrame* frame,
    const ResourceRequest& req,
    const SubstituteData& substitute_data,
    ClientRedirectPolicy client_redirect_policy,
    const base::UnguessableToken& devtools_navigation_token,
    WebFrameLoadType load_type,
    WebNavigationType navigation_type,
    std::unique_ptr<WebNavigationParams> navigation_params)
    : frame_(frame),
      fetcher_(FrameFetchContext::CreateFetcherFromDocumentLoader(this)),
      original_request_(req),
      substitute_data_(substitute_data),
      request_(req),
      load_type_(load_type),
      is_client_redirect_(client_redirect_policy ==
                          ClientRedirectPolicy::kClientRedirect),
      replaces_current_history_item_(false),
      data_received_(false),
      navigation_type_(navigation_type),
      document_load_timing_(*this),
      application_cache_host_(ApplicationCacheHost::Create(this)),
      service_worker_network_provider_(
          navigation_params
              ? std::move(navigation_params->service_worker_network_provider)
              : nullptr),
      was_blocked_after_csp_(false),
      state_(kNotStarted),
      committed_data_buffer_(nullptr),
      in_data_received_(false),
      data_buffer_(SharedBuffer::Create()),
      devtools_navigation_token_(devtools_navigation_token),
      had_sticky_activation_(navigation_params &&
                             navigation_params->is_user_activated),
      had_transient_activation_(request_.HasUserGesture()),
      use_counter_(frame_->GetChromeClient().IsSVGImageChromeClient()
                       ? UseCounter::kSVGImageContext
                       : UseCounter::kDefaultContext) {
  DCHECK(frame_);

  WebNavigationTimings timings;
  if (navigation_params)
    timings = navigation_params->navigation_timings;
  if (!timings.input_start.is_null())
    document_load_timing_.SetInputStart(timings.input_start);
  if (timings.navigation_start.is_null()) {
    document_load_timing_.SetNavigationStart(CurrentTimeTicks());
  } else {
    document_load_timing_.SetNavigationStart(timings.navigation_start);
    if (!timings.redirect_start.is_null()) {
      document_load_timing_.SetRedirectStart(timings.redirect_start);
      document_load_timing_.SetRedirectEnd(timings.redirect_end);
    }
    if (!timings.fetch_start.is_null()) {
      DCHECK(!timings.navigation_start.is_null());
      document_load_timing_.SetFetchStart(timings.fetch_start);
    }
  }

  if (navigation_params && navigation_params->source_location.has_value()) {
    WebSourceLocation& location = navigation_params->source_location.value();
    source_location_ = SourceLocation::Create(
        location.url, location.line_number, location.column_number, nullptr);
  }

  replaces_current_history_item_ =
      load_type_ == WebFrameLoadType::kReplaceCurrentItem &&
      (!frame_->Loader().Opener() || !request_.Url().IsEmpty());

  if (is_client_redirect_)
    AppendRedirect(frame_->GetDocument()->Url());
}
