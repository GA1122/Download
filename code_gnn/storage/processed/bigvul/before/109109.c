void RenderViewImpl::didReceiveDocumentData(
    WebFrame* frame, const char* data, size_t data_len,
    bool& prevent_default) {
  DocumentState* document_state =
      DocumentState::FromDataSource(frame->dataSource());
  document_state->set_use_error_page(false);
}
