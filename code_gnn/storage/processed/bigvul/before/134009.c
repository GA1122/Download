void ExtensionAppItem::ExtensionEnableFlowFinished() {
  extension_enable_flow_.reset();
  extension_enable_flow_controller_->OnCloseChildDialog();
  extension_enable_flow_controller_ = NULL;

  Launch(ui::EF_NONE);
}
