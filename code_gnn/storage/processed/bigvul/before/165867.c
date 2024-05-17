void RenderFrameImpl::BeginNavigation(
    std::unique_ptr<blink::WebNavigationInfo> info) {


  const GURL& url = info->url_request.Url();

#ifdef OS_ANDROID
  bool render_view_was_created_by_renderer =
      render_view_->was_created_by_renderer_;
  if (!IsURLHandledByNetworkStack(url) && !url.is_empty() &&
      GetContentClient()->renderer()->HandleNavigation(
          this, true  ,
          render_view_was_created_by_renderer, frame_, info->url_request,
          info->navigation_type, info->navigation_policy,
          false  )) {
    return;
  }
#endif

  if (IsTopLevelNavigation(frame_) &&
      render_view_->renderer_preferences_
          .browser_handles_all_top_level_requests) {
    OpenURL(std::move(info),  false);
    return;   
  }

  if (info->is_history_navigation_in_new_child_frame && frame_->Parent()) {
    bool should_ask_browser = false;
    RenderFrameImpl* parent = RenderFrameImpl::FromWebFrame(frame_->Parent());
    auto iter = parent->history_subframe_unique_names_.find(
        unique_name_helper_.value());
    if (iter != parent->history_subframe_unique_names_.end()) {
      bool history_item_is_about_blank = iter->second;
      should_ask_browser =
          !history_item_is_about_blank || url != url::kAboutBlankURL;
      parent->history_subframe_unique_names_.erase(iter);
    }

    if (should_ask_browser) {
      if (!info->is_client_redirect) {
        OpenURL(std::move(info),  true);
        frame_->MarkAsLoading();
        return;
      }
      GetFrameHost()->CancelInitialHistoryLoad();
    }
  }

  GURL old_url(frame_->GetDocumentLoader()->GetUrl());

  if (!frame_->Parent() && !url.SchemeIs(url::kAboutScheme) &&
      !url.is_empty()) {
    int cumulative_bindings = RenderProcess::current()->GetEnabledBindings();
    bool is_initial_navigation = render_view_->history_list_length_ == 0;
    bool should_fork =
        HasWebUIScheme(url) || HasWebUIScheme(old_url) ||
        (cumulative_bindings & kWebUIBindingsPolicyMask) ||
        url.SchemeIs(kViewSourceScheme) ||
        (frame_->IsViewSourceModeEnabled() &&
         info->navigation_type != blink::kWebNavigationTypeReload);
    if (!should_fork && url.SchemeIs(url::kFileScheme)) {
      should_fork = !old_url.SchemeIs(url::kFileScheme);
    }

    if (!should_fork) {
      should_fork = GetContentClient()->renderer()->ShouldFork(
          frame_, url, info->url_request.HttpMethod().Utf8(),
          is_initial_navigation, false  );
    }

    if (should_fork) {
      OpenURL(std::move(info),  false);
      return;   
    }
  }

  bool should_dispatch_before_unload =
      info->navigation_policy == blink::kWebNavigationPolicyCurrentTab &&
      (has_accessed_initial_document_ || !current_history_item_.IsNull());

  if (should_dispatch_before_unload) {
    base::WeakPtr<RenderFrameImpl> weak_self = weak_factory_.GetWeakPtr();

    if (!frame_->DispatchBeforeUnloadEvent(info->navigation_type ==
                                           blink::kWebNavigationTypeReload) ||
        !weak_self) {
      return;
    }
  }

  if (info->navigation_policy == blink::kWebNavigationPolicyCurrentTab) {
    if (!info->form.IsNull()) {
      for (auto& observer : observers_)
        observer.WillSubmitForm(info->form);
    }

    sync_navigation_callback_.Cancel();

    bool use_archive = (info->archive_status ==
                        blink::WebNavigationInfo::ArchiveStatus::Present) &&
                       !url.SchemeIs(url::kDataScheme);

    if (!use_archive && IsURLHandledByNetworkStack(url)) {
      BeginNavigationInternal(std::move(info));
      return;
    }

    if (WebDocumentLoader::WillLoadUrlAsEmpty(url) &&
        !frame_->HasCommittedFirstRealLoad()) {
      CommitSyncNavigation(std::move(info));
      return;
    }

    if (!CreatePlaceholderDocumentLoader(*info))
      return;
    sync_navigation_callback_.Reset(
        base::BindOnce(&RenderFrameImpl::CommitSyncNavigation,
                       weak_factory_.GetWeakPtr(), base::Passed(&info)));
    frame_->GetTaskRunner(blink::TaskType::kInternalLoading)
        ->PostTask(FROM_HERE, sync_navigation_callback_.callback());
    return;
  }

  if (info->navigation_policy == blink::kWebNavigationPolicyDownload) {
    blink::mojom::BlobURLTokenPtrInfo blob_url_token =
        CloneBlobURLToken(info->blob_url_token.get());
    DownloadURL(info->url_request,
                blink::WebLocalFrameClient::CrossOriginRedirects::kFollow,
                blob_url_token.PassHandle());
  } else {
    OpenURL(std::move(info),  false);
  }
}
