HTMLDialogElement* Document::ActiveModalDialog() const {
  if (top_layer_elements_.IsEmpty())
    return 0;
  return toHTMLDialogElement(top_layer_elements_.back().Get());
}
