ui::PageTransition GetTransitionType(blink::WebDocumentLoader* document_loader,
                                     bool is_main_frame,
                                     bool loading) {
  NavigationState* navigation_state =
      NavigationState::FromDocumentLoader(document_loader);
  ui::PageTransition default_transition =
      navigation_state->IsContentInitiated()
          ? ui::PAGE_TRANSITION_LINK
          : navigation_state->common_params().transition;
  if (navigation_state->WasWithinSameDocument())
    return default_transition;
  if (loading || document_loader->GetResponse().IsNull()) {
    return GetTransitionType(
        default_transition, document_loader->ReplacesCurrentHistoryItem(),
        is_main_frame, document_loader->GetNavigationType());
  }
  return default_transition;
}
