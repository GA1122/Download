void RenderFrameImpl::willSubmitForm(blink::WebLocalFrame* frame,
                                     const blink::WebFormElement& form) {
  DCHECK(!frame_ || frame_ == frame);
  DocumentState* document_state =
      DocumentState::FromDataSource(frame->provisionalDataSource());
  NavigationStateImpl* navigation_state =
      static_cast<NavigationStateImpl*>(document_state->navigation_state());
  InternalDocumentStateData* internal_data =
      InternalDocumentStateData::FromDocumentState(document_state);

  if (ui::PageTransitionCoreTypeIs(navigation_state->GetTransitionType(),
                                   ui::PAGE_TRANSITION_LINK)) {
    navigation_state->set_transition_type(ui::PAGE_TRANSITION_FORM_SUBMIT);
  }

  WebSearchableFormData web_searchable_form_data(form);
  internal_data->set_searchable_form_url(web_searchable_form_data.url());
  internal_data->set_searchable_form_encoding(
      web_searchable_form_data.encoding().utf8());

  FOR_EACH_OBSERVER(RenderFrameObserver, observers_, WillSubmitForm(form));
}
