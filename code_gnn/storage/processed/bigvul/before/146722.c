void Document::CreateImportsController() {
  DCHECK(!imports_controller_);
  imports_controller_ = HTMLImportsController::Create(*this);
}
