blink::mojom::CommitResult RenderFrameImpl::PrepareForHistoryNavigationCommit(
    FrameMsg_Navigate_Type::Value navigation_type,
    const RequestNavigationParams& request_params,
    WebHistoryItem* item_for_history_navigation,
    blink::WebFrameLoadType* load_type) {
  DCHECK(navigation_type == FrameMsg_Navigate_Type::HISTORY_SAME_DOCUMENT ||
         navigation_type ==
             FrameMsg_Navigate_Type::HISTORY_DIFFERENT_DOCUMENT ||
         navigation_type == FrameMsg_Navigate_Type::RESTORE ||
         navigation_type == FrameMsg_Navigate_Type::RESTORE_WITH_POST);
  std::unique_ptr<HistoryEntry> entry =
      PageStateToHistoryEntry(request_params.page_state);
  if (!entry)
    return blink::mojom::CommitResult::Aborted;

  *item_for_history_navigation = entry->root();
  *load_type = blink::WebFrameLoadType::kBackForward;

  history_subframe_unique_names_ = request_params.subframe_unique_names;

  if (navigation_type == FrameMsg_Navigate_Type::HISTORY_SAME_DOCUMENT) {
    if (current_history_item_.IsNull()) {
      NOTREACHED();
      return blink::mojom::CommitResult::RestartCrossDocument;
    }

    if (current_history_item_.DocumentSequenceNumber() !=
        item_for_history_navigation->DocumentSequenceNumber()) {
      return blink::mojom::CommitResult::RestartCrossDocument;
    }
  }

  bool interrupted_by_client_redirect =
      frame_->IsNavigationScheduledWithin(0) ||
      frame_->GetProvisionalDocumentLoader() || !current_history_item_.IsNull();
  if (request_params.is_history_navigation_in_new_child &&
      interrupted_by_client_redirect) {
    return blink::mojom::CommitResult::Aborted;
  }

  return blink::mojom::CommitResult::Ok;
}
