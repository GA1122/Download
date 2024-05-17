void Textfield::OnBeforePointerAction() {
  OnBeforeUserAction();
  if (model_->HasCompositionText())
    model_->ConfirmCompositionText();
}
