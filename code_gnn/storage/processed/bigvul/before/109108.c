void RenderViewImpl::didNavigateWithinPage(
    WebFrame* frame, bool is_new_navigation) {
  didCreateDataSource(frame, frame->dataSource());

  DocumentState* document_state =
      DocumentState::FromDataSource(frame->dataSource());
  NavigationState* new_state = document_state->navigation_state();
  new_state->set_was_within_same_page(true);

  didCommitProvisionalLoad(frame, is_new_navigation);

  WebDataSource* datasource = frame->view()->mainFrame()->dataSource();
  UpdateTitle(frame, datasource->pageTitle(), datasource->pageTitleDirection());
}
