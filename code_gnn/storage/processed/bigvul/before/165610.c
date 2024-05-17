bool Location::IsAttached() const {
  return dom_window_->GetFrame();
}
