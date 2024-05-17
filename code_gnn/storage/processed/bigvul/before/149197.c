  void SimulateTab() {
    ui::KeyEvent pressed_tab(ui::ET_KEY_PRESSED, ui::VKEY_TAB, ui::EF_NONE);
    media_controls_view_->GetFocusManager()->OnKeyEvent(pressed_tab);
  }
