void TestRenderWidgetHostView::RenderProcessGone(base::TerminationStatus status,
                                                 int error_code) {
  delete this;
}
