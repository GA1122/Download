bool BrowserView::GetAcceleratorForCommandId(int command_id,
                                             ui::Accelerator* accelerator) {
  return toolbar_->GetAcceleratorForCommandId(command_id, accelerator);
}
