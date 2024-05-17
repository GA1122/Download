bool AppResult::RunExtensionEnableFlow() {
  if (extensions::util::IsAppLaunchableWithoutEnabling(app_id_, profile_))
    return false;

  if (!extension_enable_flow_) {
    controller_->OnShowChildDialog();

    extension_enable_flow_.reset(new ExtensionEnableFlow(
        profile_, app_id_, this));
    extension_enable_flow_->StartForNativeWindow(
        controller_->GetAppListWindow());
  }
  return true;
}
