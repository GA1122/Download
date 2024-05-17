bool Document::HaveImportsLoaded() const {
  if (!imports_controller_)
    return true;
  return !imports_controller_->ShouldBlockScriptExecution(*this);
}
