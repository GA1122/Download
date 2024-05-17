void LockContentsView::RegisterAccelerators() {
  accel_map_[ui::Accelerator(ui::VKEY_I, ui::EF_SHIFT_DOWN | ui::EF_ALT_DOWN)] =
      AcceleratorAction::kShowFeedback;

  AcceleratorController* controller = Shell::Get()->accelerator_controller();
  for (const auto& item : accel_map_)
    controller->Register({item.first}, this);
}
