void RenderFrameImpl::DidFinishSameDocumentNavigation(
    const blink::WebHistoryItem& item,
    blink::WebHistoryCommitType commit_type,
    bool content_initiated) {
  TRACE_EVENT1("navigation,rail",
               "RenderFrameImpl::didFinishSameDocumentNavigation", "id",
               routing_id_);
  InternalDocumentStateData* data =
      InternalDocumentStateData::FromDocumentLoader(
          frame_->GetDocumentLoader());
  if (content_initiated)
    data->set_navigation_state(NavigationState::CreateContentInitiated());
  data->navigation_state()->set_was_within_same_document(true);

  ui::PageTransition transition = GetTransitionType(frame_->GetDocumentLoader(),
                                                    frame_, true  );
  DidCommitNavigationInternal(item, commit_type,
                              true  , transition,
                              nullptr  );
}
