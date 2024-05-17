bool HTMLInputElement::SizeShouldIncludeDecoration(int& preferred_size) const {
  return input_type_view_->SizeShouldIncludeDecoration(kDefaultSize,
                                                       preferred_size);
}
