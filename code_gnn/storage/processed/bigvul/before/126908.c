bool BrowserTabStripController::IsActiveTab(int model_index) const {
  return model_->active_index() == model_index;
}
