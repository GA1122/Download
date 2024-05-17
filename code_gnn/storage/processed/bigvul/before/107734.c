void FrameLoader::CommitNavigation(
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

  navigation_params->frame_load_type = DetermineFrameLoadType(
      navigation_params->url, navigation_params->http_method,
      nullptr  , navigation_params->unreachable_url,
      navigation_params->frame_load_type);


  RecordLatestRequiredCSP();

  if (!CancelProvisionalLoaderForNewNavigation(
          false  ,
          DocumentLoader::WillLoadUrlAsEmpty(navigation_params->url))) {
    return;
  }

  WebNavigationType navigation_type = DetermineNavigationType(
      navigation_params->frame_load_type,
      !navigation_params->http_body.IsNull(), false  );

  HistoryItem* history_item = nullptr;
  if (IsBackForwardLoadType(navigation_params->frame_load_type)) {
    history_item = navigation_params->history_item;
    DCHECK(history_item);
  }

  provisional_document_loader_ = Client()->CreateDocumentLoader(
      frame_, navigation_type, std::move(navigation_params),
      std::move(extra_data));
  if (history_item)
    provisional_document_loader_->SetItemForHistoryNavigation(history_item);

  frame_->GetFrameScheduler()->DidStartProvisionalLoad(frame_->IsMainFrame());
  Client()->DispatchDidStartProvisionalLoad(provisional_document_loader_);
  probe::DidStartProvisionalLoad(frame_);
  virtual_time_pauser_.PauseVirtualTime();

  provisional_document_loader_->StartLoading();
  TakeObjectSnapshot();
}
