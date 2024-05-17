void HTMLInputElement::FinishParsingChildren() {
  parsing_in_progress_ = false;
  DCHECK(input_type_);
  DCHECK(input_type_view_);
  TextControlElement::FinishParsingChildren();
  if (!state_restored_) {
    bool checked = hasAttribute(checkedAttr);
    if (checked)
      setChecked(checked);
    dirty_checkedness_ = false;
  }
}
