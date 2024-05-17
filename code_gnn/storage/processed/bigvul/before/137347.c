  void NonClientMouseClick() {
    ui::MouseEvent click(ui::ET_MOUSE_PRESSED, gfx::Point(), gfx::Point(),
                         ui::EventTimeForNow(),
                         ui::EF_LEFT_MOUSE_BUTTON | ui::EF_IS_NON_CLIENT,
                         ui::EF_LEFT_MOUSE_BUTTON);
    textfield_->OnMousePressed(click);
    ui::MouseEvent release(ui::ET_MOUSE_RELEASED, gfx::Point(), gfx::Point(),
                           ui::EventTimeForNow(),
                           ui::EF_LEFT_MOUSE_BUTTON | ui::EF_IS_NON_CLIENT,
                           ui::EF_LEFT_MOUSE_BUTTON);
    textfield_->OnMouseReleased(release);
  }
