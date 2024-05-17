bool HTMLInputElement::LayoutObjectIsNeeded(const ComputedStyle& style) {
  return input_type_->LayoutObjectIsNeeded() &&
         TextControlElement::LayoutObjectIsNeeded(style);
}
