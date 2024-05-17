void BlinkTestRunner::UseUnfortunateSynchronousResizeMode(bool enable) {
  UseSynchronousResizeModeVisitor visitor(enable);
  RenderView::ForEach(&visitor);
}
