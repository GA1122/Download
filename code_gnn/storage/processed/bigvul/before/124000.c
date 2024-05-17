void RenderViewImpl::willSubmitForm(WebFrame* frame,
                                    const WebFormElement& form) {
  DocumentState* document_state =
      DocumentState::FromDataSource(frame->provisionalDataSource());
  NavigationState* navigation_state = document_state->navigation_state();

  if (navigation_state->transition_type() == PAGE_TRANSITION_LINK)
    navigation_state->set_transition_type(PAGE_TRANSITION_FORM_SUBMIT);

  WebSearchableFormData web_searchable_form_data(form);
  document_state->set_searchable_form_url(web_searchable_form_data.url());
  document_state->set_searchable_form_encoding(
      web_searchable_form_data.encoding().utf8());
  scoped_ptr<PasswordForm> password_form_data =
      CreatePasswordForm(form);

  if (password_form_data.get()) {
    DocumentState* old_document_state =
        DocumentState::FromDataSource(frame->dataSource());
    if (old_document_state) {
      PasswordForm* old_form_data = old_document_state->password_form_data();
      if (old_form_data && old_form_data->action == password_form_data->action)
        password_form_data->password_value = old_form_data->password_value;
    }
  }

  document_state->set_password_form_data(password_form_data.Pass());

  FOR_EACH_OBSERVER(
      RenderViewObserver, observers_, WillSubmitForm(frame, form));
}
