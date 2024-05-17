  void DragMouseTo(const gfx::Point& where) {
    mouse_position_ = where;
    ui::MouseEvent drag(ui::ET_MOUSE_DRAGGED, where, where,
                        ui::EventTimeForNow(), ui::EF_LEFT_MOUSE_BUTTON, 0);
    textfield_->OnMouseDragged(drag);
  }
