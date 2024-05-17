bool SelectFileDialogImpl::IsRunning(HWND owning_hwnd) const {
  return listener_ && IsRunningDialogForOwner(owning_hwnd);
}
