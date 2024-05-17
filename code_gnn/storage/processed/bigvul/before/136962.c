void HTMLInputElement::SetInnerEditorValue(const String& value) {
  TextControlElement::SetInnerEditorValue(value);
  needs_to_update_view_value_ = false;
}
