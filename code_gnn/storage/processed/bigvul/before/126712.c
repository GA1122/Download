bool BrowserView::AcceleratorPressed(const ui::Accelerator& accelerator) {
#if defined(OS_CHROMEOS)
  if (accelerator.key_code() == ui::VKEY_LWIN &&
      g_browser_process->local_state()->GetBoolean(
          prefs::kSpokenFeedbackEnabled)) {
    ExtensionTtsController::GetInstance()->Stop();
    return false;
  }
#endif

  std::map<ui::Accelerator, int>::const_iterator iter =
      accelerator_table_.find(accelerator);
  DCHECK(iter != accelerator_table_.end());
  int command_id = iter->second;

  chrome::BrowserCommandController* controller = browser_->command_controller();
  if (!controller->block_command_execution())
    UpdateAcceleratorMetrics(accelerator, command_id);
  return chrome::ExecuteCommand(browser_.get(), command_id);
}
