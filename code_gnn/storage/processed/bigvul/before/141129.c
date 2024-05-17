bool Document::IsHTMLImport() const {
  return imports_controller_ && imports_controller_->Master() != this;
}
