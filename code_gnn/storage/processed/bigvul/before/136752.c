void LocalDOMWindow::FinishedLoading() {
  if (should_print_when_finished_loading_) {
    should_print_when_finished_loading_ = false;
    print(nullptr);
  }
}
