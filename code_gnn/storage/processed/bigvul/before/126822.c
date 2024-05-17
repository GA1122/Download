void BrowserView::LoadAccelerators() {
#if defined(OS_WIN) && !defined(USE_AURA)
  HACCEL accelerator_table = AtlLoadAccelerators(IDR_MAINFRAME);
  DCHECK(accelerator_table);

  int count = CopyAcceleratorTable(accelerator_table, 0, 0);
  if (count == 0) {
    return;
  }

  ACCEL* accelerators = static_cast<ACCEL*>(malloc(sizeof(ACCEL) * count));
  CopyAcceleratorTable(accelerator_table, accelerators, count);

  views::FocusManager* focus_manager = GetFocusManager();
  DCHECK(focus_manager);

  for (int i = 0; i < count; ++i) {
    ui::Accelerator accelerator(
        static_cast<ui::KeyboardCode>(accelerators[i].key),
        ui::GetModifiersFromACCEL(accelerators[i]));
    accelerator_table_[accelerator] = accelerators[i].cmd;

    focus_manager->RegisterAccelerator(
        accelerator, ui::AcceleratorManager::kNormalPriority, this);
  }

  free(accelerators);
#else
  views::FocusManager* focus_manager = GetFocusManager();
  DCHECK(focus_manager);
  for (size_t i = 0; i < chrome::kAcceleratorMapLength; ++i) {
    ui::Accelerator accelerator(chrome::kAcceleratorMap[i].keycode,
                                chrome::kAcceleratorMap[i].modifiers);
    accelerator_table_[accelerator] = chrome::kAcceleratorMap[i].command_id;

    focus_manager->RegisterAccelerator(
        accelerator, ui::AcceleratorManager::kNormalPriority, this);
  }
#endif
}
