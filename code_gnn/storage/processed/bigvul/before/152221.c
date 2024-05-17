void RenderFrameImpl::CommitSameDocumentNavigation(
    const CommonNavigationParams& common_params,
    const CommitNavigationParams& commit_params,
    CommitSameDocumentNavigationCallback callback) {
  DCHECK(!IsRendererDebugURL(common_params.url));
  DCHECK(!FrameMsg_Navigate_Type::IsReload(common_params.navigation_type));
  DCHECK(!commit_params.is_view_source);
  DCHECK(FrameMsg_Navigate_Type::IsSameDocument(common_params.navigation_type));

  PrepareFrameForCommit(common_params.url, commit_params);

  blink::WebFrameLoadType load_type = NavigationTypeToLoadType(
      common_params.navigation_type, common_params.should_replace_current_entry,
      commit_params.page_state.IsValid());

  blink::mojom::CommitResult commit_status = blink::mojom::CommitResult::Ok;
  WebHistoryItem item_for_history_navigation;

  if (common_params.navigation_type ==
      FrameMsg_Navigate_Type::HISTORY_SAME_DOCUMENT) {
    DCHECK(commit_params.page_state.IsValid());
    DCHECK_NE(0, commit_params.nav_entry_id);
    DCHECK(!commit_params.is_history_navigation_in_new_child);
    commit_status = PrepareForHistoryNavigationCommit(
        common_params.navigation_type, commit_params,
        &item_for_history_navigation, &load_type);
  }

  if (commit_status == blink::mojom::CommitResult::Ok) {
    base::WeakPtr<RenderFrameImpl> weak_this = weak_factory_.GetWeakPtr();
    bool is_client_redirect =
        !!(common_params.transition & ui::PAGE_TRANSITION_CLIENT_REDIRECT);
    DocumentState* original_document_state =
        DocumentState::FromDocumentLoader(frame_->GetDocumentLoader());
    std::unique_ptr<DocumentState> document_state =
        original_document_state->Clone();
    InternalDocumentStateData* internal_data =
        InternalDocumentStateData::FromDocumentState(document_state.get());
    internal_data->CopyFrom(
        InternalDocumentStateData::FromDocumentState(original_document_state));
    internal_data->set_navigation_state(NavigationState::CreateBrowserInitiated(
        common_params, commit_params,
        base::TimeTicks(),   
        mojom::FrameNavigationControl::CommitNavigationCallback(),
        mojom::NavigationClient::CommitNavigationCallback(), nullptr,
        false  ));

    commit_status = frame_->CommitSameDocumentNavigation(
        common_params.url, load_type, item_for_history_navigation,
        is_client_redirect, std::move(document_state));

    if (!weak_this)
      return;
  }

  std::move(callback).Run(commit_status);

  if (frame_ && !frame_->IsLoading() &&
      commit_status != blink::mojom::CommitResult::Ok) {
    Send(new FrameHostMsg_DidStopLoading(routing_id_));
  }
}
