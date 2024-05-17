void DocumentLoader::UpdateForSameDocumentNavigation(
    const KURL& new_url,
    SameDocumentNavigationSource same_document_navigation_source,
    scoped_refptr<SerializedScriptValue> data,
    HistoryScrollRestorationType scroll_restoration_type,
    FrameLoadType type,
    Document* initiating_document) {
  if (type == kFrameLoadTypeStandard && initiating_document &&
      !initiating_document->CanCreateHistoryEntry()) {
    type = kFrameLoadTypeReplaceCurrentItem;
  }

  KURL old_url = request_.Url();
  original_request_.SetURL(new_url);
  request_.SetURL(new_url);
  SetReplacesCurrentHistoryItem(type != kFrameLoadTypeStandard);
  if (same_document_navigation_source == kSameDocumentNavigationHistoryApi) {
    request_.SetHTTPMethod(HTTPNames::GET);
    request_.SetHTTPBody(nullptr);
  }
  ClearRedirectChain();
  if (is_client_redirect_)
    AppendRedirect(old_url);
  AppendRedirect(new_url);

  SetHistoryItemStateForCommit(
      history_item_.Get(), type,
      same_document_navigation_source == kSameDocumentNavigationHistoryApi
          ? HistoryNavigationType::kHistoryApi
          : HistoryNavigationType::kFragment);
  history_item_->SetDocumentState(frame_->GetDocument()->FormElementsState());
  if (same_document_navigation_source == kSameDocumentNavigationHistoryApi) {
    history_item_->SetStateObject(std::move(data));
    history_item_->SetScrollRestorationType(scroll_restoration_type);
  }
  HistoryCommitType commit_type = LoadTypeToCommitType(type);
  frame_->FrameScheduler()->DidCommitProvisionalLoad(
      commit_type == kHistoryInertCommit, type == kFrameLoadTypeReload,
      frame_->IsLocalRoot());
  GetLocalFrameClient().DispatchDidNavigateWithinPage(
      history_item_.Get(), commit_type, initiating_document);
}
