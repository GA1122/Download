void Textfield::InsertText(const base::string16& new_text) {
  if (GetTextInputType() == ui::TEXT_INPUT_TYPE_NONE || new_text.empty())
    return;

  OnBeforeUserAction();
  skip_input_method_cancel_composition_ = true;
  model_->InsertText(new_text);
  skip_input_method_cancel_composition_ = false;
  UpdateAfterChange(true, true);
  OnAfterUserAction();
}
