void LocalDOMWindow::scrollTo(double x, double y) const {
  ScrollToOptions options;
  options.setLeft(x);
  options.setTop(y);
  scrollTo(options);
}
