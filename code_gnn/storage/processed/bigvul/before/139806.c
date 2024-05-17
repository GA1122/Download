void Shell::DeleteInstance() {
  delete instance_;
  instance_ = nullptr;
}
