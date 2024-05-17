void HTMLInputElement::RestoreFormControlState(const FormControlState& state) {
  input_type_view_->RestoreFormControlState(state);
  state_restored_ = true;
}
