 bool CrosLibrary::EnsureLoaded() {
   if (use_stub_impl_)
    return true;

  if (!loaded_ && !load_error_) {
    if (!library_loader_) {
      library_loader_ = new CrosLibraryLoader();
      own_library_loader_ = true;
    }
    loaded_ = library_loader_->Load(&load_error_string_);
    load_error_ = !loaded_;
  }
  return loaded_;
}
