TestAddAppWindowObserver::TestAddAppWindowObserver(
    extensions::AppWindowRegistry* registry)
    : registry_(registry), window_(NULL) {
  registry_->AddObserver(this);
}
