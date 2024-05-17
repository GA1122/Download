bool BrowserView::AcceleratorPressed(const ui::Accelerator& accelerator) {
  int command_id;
  if (!FindCommandIdForAccelerator(accelerator, &command_id))
    return false;

  UpdateAcceleratorMetrics(accelerator, command_id);
  return chrome::ExecuteCommand(browser_.get(), command_id);
}
