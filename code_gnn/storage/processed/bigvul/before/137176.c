bool Textfield::GetAcceleratorForCommandId(int command_id,
                                           ui::Accelerator* accelerator) const {
  switch (command_id) {
    case IDS_APP_UNDO:
      *accelerator = ui::Accelerator(ui::VKEY_Z, kPlatformModifier);
      return true;

    case IDS_APP_CUT:
      *accelerator = ui::Accelerator(ui::VKEY_X, kPlatformModifier);
      return true;

    case IDS_APP_COPY:
      *accelerator = ui::Accelerator(ui::VKEY_C, kPlatformModifier);
      return true;

    case IDS_APP_PASTE:
      *accelerator = ui::Accelerator(ui::VKEY_V, kPlatformModifier);
      return true;

    case IDS_APP_SELECT_ALL:
      *accelerator = ui::Accelerator(ui::VKEY_A, kPlatformModifier);
      return true;

    default:
      return false;
  }
}
