bool BrowserTabStripController::IsValidIndex(int index) const {
  return model_->ContainsIndex(index);
}
