HTMLImportsController* Document::EnsureImportsController() {
  if (!imports_controller_) {
    DCHECK(frame_);
    imports_controller_ = HTMLImportsController::Create(*this);
  }

  return imports_controller_;
}
