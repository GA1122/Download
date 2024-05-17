DocumentState* Document::FormElementsState() const {
  if (!form_controller_)
    return 0;
  return form_controller_->FormElementsState();
}
