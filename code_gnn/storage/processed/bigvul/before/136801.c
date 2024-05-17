External* LocalDOMWindow::external() {
  if (!external_)
    external_ = new External;
  return external_;
}
