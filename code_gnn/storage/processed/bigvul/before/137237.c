bool Textfield::IsIMEComposing() const {
  return model_->HasCompositionText();
}
