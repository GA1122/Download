bool MaybeGetOverriddenURL(WebDocumentLoader* document_loader, GURL* output) {
  DocumentState* document_state =
      DocumentState::FromDocumentLoader(document_loader);

  if (document_state->was_load_data_with_base_url_request()) {
    *output = document_state->data_url();
    return true;
  }

  if (document_loader->HasUnreachableURL()) {
    *output = document_loader->UnreachableURL();
    return true;
  }

  return false;
}
