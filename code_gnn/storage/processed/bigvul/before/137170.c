void Textfield::DoInsertChar(base::char16 ch) {
  OnBeforeUserAction();
  skip_input_method_cancel_composition_ = true;
  model_->InsertChar(ch);
  skip_input_method_cancel_composition_ = false;

  UpdateAfterChange(true, true);
  OnAfterUserAction();
}
