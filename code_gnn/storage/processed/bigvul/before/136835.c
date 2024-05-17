void LocalDOMWindow::scrollBy(double x, double y) const {
  ScrollToOptions options;
  options.setLeft(x);
  options.setTop(y);
  scrollBy(options);
}
