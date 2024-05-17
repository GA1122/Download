void AppListControllerDelegateImpl::ShowForProfileByPath(
    const base::FilePath& profile_path) {
  service_->SetProfilePath(profile_path);
  service_->Show();
}
