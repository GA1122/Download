void ExtensionAppItem::ExtensionEnableFlowAborted(bool user_initiated) {
  extension_enable_flow_.reset();
  extension_enable_flow_controller_->OnCloseChildDialog();
  extension_enable_flow_controller_ = NULL;
}
