void FileManagerBrowserTestBase::TearDownOnMainThread() {
  file_tasks_observer_.reset();
  select_factory_ = nullptr;
  ui::SelectFileDialog::SetFactory(nullptr);
}
