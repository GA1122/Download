mojom::CommitResult FrameLoader::CommitSameDocumentNavigation(
    const KURL& url,
    FrameLoadType frame_load_type,
    HistoryItem* history_item,
    ClientRedirectPolicy client_redirect_policy,
    Document* origin_document,
    Event* triggering_event) {
  DCHECK(!IsReloadLoadType(frame_load_type));
  DCHECK(frame_->GetDocument());

  if (in_stop_all_loaders_)
    return mojom::CommitResult::Aborted;

  bool history_navigation = IsBackForwardLoadType(frame_load_type);

  if (!frame_->IsNavigationAllowed() && history_navigation)
    return mojom::CommitResult::Aborted;

  if (!history_navigation) {
    if (!url.HasFragmentIdentifier() ||
        !EqualIgnoringFragmentIdentifier(frame_->GetDocument()->Url(), url) ||
        frame_->GetDocument()->IsFrameSet()) {
      return mojom::CommitResult::RestartCrossDocument;
    }
  }

  DCHECK(history_item || !history_navigation);
  scoped_refptr<SerializedScriptValue> state_object =
      history_navigation ? history_item->StateObject() : nullptr;

  if (!history_navigation) {
    document_loader_->SetNavigationType(
        DetermineNavigationType(frame_load_type, false, triggering_event));
    if (ShouldTreatURLAsSameAsCurrent(url))
      frame_load_type = kFrameLoadTypeReplaceCurrentItem;
  }

  LoadInSameDocument(url, state_object, frame_load_type, history_item,
                     client_redirect_policy, origin_document);
  return mojom::CommitResult::Ok;
}
