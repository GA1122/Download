void OverlayWindowViews::OnKeyEvent(ui::KeyEvent* event) {
  hide_controls_timer_.Reset();

  if (event->type() == ui::ET_KEY_PRESSED ||
      event->key_code() == ui::VKEY_TAB) {
    UpdateControlsVisibility(true);
  }

#if defined(OS_MACOSX)
  if (event->type() == ui::ET_KEY_PRESSED &&
      event->key_code() == ui::VKEY_SPACE) {
    TogglePlayPause();
    event->SetHandled();
  }
#endif   

#if defined(OS_WIN)
  if (event->type() == ui::ET_KEY_PRESSED && event->IsAltDown() &&
      event->key_code() == ui::VKEY_F4) {
    controller_->Close(true  ,
                       true  );
    event->SetHandled();
  }
#endif   

  views::Widget::OnKeyEvent(event);
}
