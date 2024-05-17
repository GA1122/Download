void HTMLInputElement::CloneNonAttributePropertiesFrom(const Element& source,
                                                       CloneChildrenFlag flag) {
  const HTMLInputElement& source_element = ToHTMLInputElement(source);

  non_attribute_value_ = source_element.non_attribute_value_;
  has_dirty_value_ = source_element.has_dirty_value_;
  setChecked(source_element.is_checked_);
  dirty_checkedness_ = source_element.dirty_checkedness_;
  is_indeterminate_ = source_element.is_indeterminate_;
  input_type_->CopyNonAttributeProperties(source_element);

  TextControlElement::CloneNonAttributePropertiesFrom(source, flag);

  needs_to_update_view_value_ = true;
  input_type_view_->UpdateView();
}
