LayoutObject* HTMLInputElement::CreateLayoutObject(const ComputedStyle& style) {
  return input_type_view_->CreateLayoutObject(style);
}
