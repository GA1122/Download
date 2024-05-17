ui::PageTransition GetTransitionType(blink::WebDocumentLoader* document_loader,
                                     blink::WebLocalFrame* frame,
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
    if (document_loader->ReplacesCurrentHistoryItem() && frame->Parent()) {
      return ui::PAGE_TRANSITION_AUTO_SUBFRAME;
    }
    bool is_form_submit = document_loader->GetNavigationType() ==
                              blink::kWebNavigationTypeFormSubmitted ||
                          document_loader->GetNavigationType() ==
                              blink::kWebNavigationTypeFormResubmitted;
    if (ui::PageTransitionCoreTypeIs(default_transition,
                                     ui::PAGE_TRANSITION_LINK) &&
        is_form_submit) {
      return ui::PAGE_TRANSITION_FORM_SUBMIT;
    }
  }
  return default_transition;
}
