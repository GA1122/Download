bool BrowserView::GetAcceleratorForCommandId(
    int command_id,
    ui::Accelerator* accelerator) const {
  return toolbar_->GetAcceleratorForCommandId(command_id, accelerator);
}
