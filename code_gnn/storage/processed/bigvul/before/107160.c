CrosLibrary::~CrosLibrary() {
  if (own_library_loader_)
    delete library_loader_;
}
