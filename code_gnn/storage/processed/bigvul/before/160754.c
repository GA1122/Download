void RenderFrameImpl::WillSubmitForm(const blink::WebFormElement& form) {
  DocumentState* document_state =
      DocumentState::FromDocumentLoader(frame_->GetProvisionalDocumentLoader());
  NavigationStateImpl* navigation_state =
      static_cast<NavigationStateImpl*>(document_state->navigation_state());
  InternalDocumentStateData* internal_data =
      InternalDocumentStateData::FromDocumentState(document_state);

  if (ui::PageTransitionCoreTypeIs(navigation_state->GetTransitionType(),
                                   ui::PAGE_TRANSITION_LINK)) {
    navigation_state->set_transition_type(ui::PAGE_TRANSITION_FORM_SUBMIT);
  }

  WebSearchableFormData web_searchable_form_data(form);
  internal_data->set_searchable_form_url(web_searchable_form_data.Url());
  internal_data->set_searchable_form_encoding(
      web_searchable_form_data.Encoding().Utf8());

  {
    SCOPED_UMA_HISTOGRAM_TIMER("RenderFrameObservers.WillSubmitForm");
    for (auto& observer : observers_)
      observer.WillSubmitForm(form);
  }
}
