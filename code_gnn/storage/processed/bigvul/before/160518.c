void WebContentsImpl::OnSavePage() {
  if (!IsSavable()) {
    RecordSavePackageEvent(SAVE_PACKAGE_DOWNLOAD_ON_NON_HTML);
    SaveFrame(GetLastCommittedURL(), Referrer());
    return;
  }

  Stop();

  save_package_ = new SavePackage(this);
  save_package_->GetSaveInfo();
}
