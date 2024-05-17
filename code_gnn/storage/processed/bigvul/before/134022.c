bool ExtensionAppItem::RunExtensionEnableFlow() {
  if (extensions::util::IsAppLaunchableWithoutEnabling(extension_id_, profile_))
    return false;

  if (!extension_enable_flow_) {
    extension_enable_flow_controller_ = GetController();
    extension_enable_flow_controller_->OnShowChildDialog();

    extension_enable_flow_.reset(new ExtensionEnableFlow(
        profile_, extension_id_, this));
    extension_enable_flow_->StartForNativeWindow(
        extension_enable_flow_controller_->GetAppListWindow());
  }
  return true;
}
