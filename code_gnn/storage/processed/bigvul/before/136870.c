scoped_refptr<ComputedStyle> HTMLInputElement::CustomStyleForLayoutObject() {
  return input_type_view_->CustomStyleForLayoutObject(
      OriginalStyleForLayoutObject());
}
