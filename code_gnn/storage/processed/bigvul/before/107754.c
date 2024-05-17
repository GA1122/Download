void FrameLoader::LoadInSameDocument(
    const KURL& url,
    scoped_refptr<SerializedScriptValue> state_object,
    WebFrameLoadType frame_load_type,
    HistoryItem* history_item,
    ClientRedirectPolicy client_redirect,
    Document* initiating_document,
    std::unique_ptr<WebDocumentLoader::ExtraData> extra_data) {
  DCHECK(!state_object || frame_load_type == WebFrameLoadType::kBackForward);

  DetachDocumentLoader(provisional_document_loader_);

  if (!frame_->GetPage())
    return;
  SaveScrollState();

  KURL old_url = frame_->GetDocument()->Url();
  bool hash_change = EqualIgnoringFragmentIdentifier(url, old_url) &&
                     url.FragmentIdentifier() != old_url.FragmentIdentifier();
  if (hash_change) {
    frame_->GetEventHandler().StopAutoscroll();
    frame_->DomWindow()->EnqueueHashchangeEvent(old_url, url);
  }
  document_loader_->SetIsClientRedirect(client_redirect ==
                                        ClientRedirectPolicy::kClientRedirect);
  if (history_item)
    document_loader_->SetItemForHistoryNavigation(history_item);
  if (extra_data)
    Client()->UpdateDocumentLoader(document_loader_, std::move(extra_data));
  UpdateForSameDocumentNavigation(url, kSameDocumentNavigationDefault, nullptr,
                                  kScrollRestorationAuto, frame_load_type,
                                  initiating_document);

  document_loader_->GetInitialScrollState().was_scrolled_by_user = false;

  frame_->GetDocument()->CheckCompleted();

  base::Optional<HistoryItem::ViewState> view_state;
  if (history_item) {
    view_state = history_item->GetViewState();
  }

  frame_->DomWindow()->StatePopped(state_object
                                       ? std::move(state_object)
                                       : SerializedScriptValue::NullValue());

  if (view_state) {
    RestoreScrollPositionAndViewState(frame_load_type,
                                      true  , *view_state,
                                      history_item->ScrollRestorationType());
  }

  ProcessFragment(url, frame_load_type, kNavigationWithinSameDocument);

  TakeObjectSnapshot();
}
