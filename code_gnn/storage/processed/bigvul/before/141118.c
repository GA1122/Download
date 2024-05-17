HTMLImportLoader* Document::ImportLoader() const {
  if (!imports_controller_)
    return nullptr;
  return imports_controller_->LoaderFor(*this);
}
