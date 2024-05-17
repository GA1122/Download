void RenderView::DidFlushPaint() {
  pepper_delegate_.ViewFlushedPaint();

  WebFrame* main_frame = webview()->mainFrame();

  if (!main_frame->provisionalDataSource()) {
    WebDataSource* ds = main_frame->dataSource();
    NavigationState* navigation_state = NavigationState::FromDataSource(ds);
    DCHECK(navigation_state);

    Time now = Time::Now();
    if (navigation_state->first_paint_time().is_null()) {
      navigation_state->set_first_paint_time(now);
    }
    if (navigation_state->first_paint_after_load_time().is_null() &&
        !navigation_state->finish_load_time().is_null()) {
      navigation_state->set_first_paint_after_load_time(now);
    }
  }
}
