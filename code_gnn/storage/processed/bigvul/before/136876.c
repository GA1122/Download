void HTMLInputElement::DetachLayoutTree(const AttachContext& context) {
  if (GetLayoutObject()) {
    input_type_->OnDetachWithLayoutObject();
  }
  TextControlElement::DetachLayoutTree(context);
  needs_to_update_view_value_ = true;
  input_type_view_->ClosePopupView();
}
