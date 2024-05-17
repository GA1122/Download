void Textfield::ClearSelection() {
  model_->ClearSelection();
  UpdateAfterChange(false, true);
}
