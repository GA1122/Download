void FrameLoader::CommitNavigation(
    const ResourceRequest& request,
    const SubstituteData& substitute_data,
    ClientRedirectPolicy client_redirect_policy,
    const base::UnguessableToken& devtools_navigation_token,
    WebFrameLoadType frame_load_type,
    HistoryItem* history_item,
    std::unique_ptr<WebNavigationParams> navigation_params,
    std::unique_ptr<WebDocumentLoader::ExtraData> extra_data) {
  DCHECK(frame_->GetDocument());
  DCHECK(Client()->HasWebView());

  if (in_stop_all_loaders_ || !frame_->IsNavigationAllowed() ||
      frame_->GetDocument()->PageDismissalEventBeingDispatched() !=
          Document::kNoDismissal) {
    return;
  }

  if (HTMLFrameOwnerElement* element = frame_->DeprecatedLocalOwner())
    element->CancelPendingLazyLoad();

  ResourceRequest resource_request = request;
  resource_request.SetHasUserGesture(
      LocalFrame::HasTransientUserActivation(frame_));
  resource_request.SetFetchRequestMode(
      network::mojom::FetchRequestMode::kNavigate);
  resource_request.SetFetchCredentialsMode(
      network::mojom::FetchCredentialsMode::kInclude);
  resource_request.SetFetchRedirectMode(
      network::mojom::FetchRedirectMode::kManual);

  frame_load_type =
      DetermineFrameLoadType(resource_request, nullptr  ,
                             substitute_data.FailingURL(), frame_load_type);


  RecordLatestRequiredCSP();

  if (!CancelProvisionalLoaderForNewNavigation(
          false  )) {
    return;
  }

  WebNavigationType navigation_type = DetermineNavigationType(
      frame_load_type, resource_request.HttpBody(), false  );
  resource_request.SetRequestContext(
      DetermineRequestContextFromNavigationType(navigation_type));
  resource_request.SetFrameType(
      frame_->IsMainFrame() ? network::mojom::RequestContextFrameType::kTopLevel
                            : network::mojom::RequestContextFrameType::kNested);

  provisional_document_loader_ = CreateDocumentLoader(
      resource_request, substitute_data, client_redirect_policy,
      devtools_navigation_token, frame_load_type, navigation_type,
      std::move(navigation_params), std::move(extra_data));
  provisional_document_loader_->AppendRedirect(
      provisional_document_loader_->Url());
  if (IsBackForwardLoadType(frame_load_type)) {
    DCHECK(history_item);
    provisional_document_loader_->SetItemForHistoryNavigation(history_item);
  }

  frame_->GetFrameScheduler()->DidStartProvisionalLoad(frame_->IsMainFrame());
  Client()->DispatchDidStartProvisionalLoad(provisional_document_loader_,
                                            resource_request);
  probe::didStartProvisionalLoad(frame_);
  virtual_time_pauser_.PauseVirtualTime();

  provisional_document_loader_->StartLoading();
  TakeObjectSnapshot();
}
