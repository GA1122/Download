  void MouseClick(const gfx::Rect bound, int x_offset) {
    gfx::Point point(bound.x() + x_offset, bound.y() + bound.height() / 2);
    ui::MouseEvent click(ui::ET_MOUSE_PRESSED, point, point,
                         ui::EventTimeForNow(), ui::EF_LEFT_MOUSE_BUTTON,
                         ui::EF_LEFT_MOUSE_BUTTON);
    textfield_->OnMousePressed(click);
    ui::MouseEvent release(ui::ET_MOUSE_RELEASED, point, point,
                           ui::EventTimeForNow(), ui::EF_LEFT_MOUSE_BUTTON,
                           ui::EF_LEFT_MOUSE_BUTTON);
    textfield_->OnMouseReleased(release);
  }
