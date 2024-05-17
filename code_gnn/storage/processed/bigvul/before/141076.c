DocumentState* Document::FormElementsState() const {
  if (!form_controller_)
    return nullptr;
  return form_controller_->FormElementsState();
}
