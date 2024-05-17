Document* Location::GetDocument() const {
  return ToLocalDOMWindow(dom_window_)->document();
}
