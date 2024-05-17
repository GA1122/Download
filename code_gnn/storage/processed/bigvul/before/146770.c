HTMLImportLoader* Document::ImportLoader() const {
  if (!imports_controller_)
    return 0;
  return imports_controller_->LoaderFor(*this);
}
