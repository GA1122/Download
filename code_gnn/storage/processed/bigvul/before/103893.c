void RenderView::didNavigateWithinPage(
    WebFrame* frame, bool is_new_navigation) {
  didCreateDataSource(frame, frame->dataSource());

  NavigationState* new_state =
      NavigationState::FromDataSource(frame->dataSource());
  new_state->set_was_within_same_page(true);

  didCommitProvisionalLoad(frame, is_new_navigation);

  UpdateTitle(frame, frame->view()->mainFrame()->dataSource()->pageTitle());
}
