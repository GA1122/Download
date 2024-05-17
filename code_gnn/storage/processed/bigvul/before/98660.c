void RenderWidget::UpdateIME() {
  if (!ime_is_active_) {
    return;
  }
  bool enable_ime = false;
  WebRect caret_rect;
  if (!webwidget_ ||
      !webwidget_->queryCompositionStatus(&enable_ime, &caret_rect)) {
    ime_control_updated_ = true;
    ime_control_new_state_ = false;
  }
  if (ime_control_new_state_ != enable_ime) {
    ime_control_updated_ = true;
    ime_control_new_state_ = enable_ime;
  }
  if (ime_control_updated_) {
    if (ime_control_enable_ime_) {
      if (ime_control_new_state_) {
        Send(new ViewHostMsg_ImeUpdateStatus(routing_id(),
                                             IME_COMPLETE_COMPOSITION,
                                             caret_rect));
      } else {
        Send(new ViewHostMsg_ImeUpdateStatus(routing_id(), IME_DISABLE,
                                             caret_rect));
      }
    } else {
      if (ime_control_new_state_) {
        Send(new ViewHostMsg_ImeUpdateStatus(routing_id(),
                                             IME_COMPLETE_COMPOSITION,
                                             caret_rect));
      } else {
      }
    }
  } else {
    if (ime_control_enable_ime_) {
      if (caret_rect.x != ime_control_x_ ||
          caret_rect.y != ime_control_y_) {
        Send(new ViewHostMsg_ImeUpdateStatus(routing_id(), IME_MOVE_WINDOWS,
                                             caret_rect));
      }
    }
  }
  ime_control_updated_ = false;
  ime_control_enable_ime_ = ime_control_new_state_;
  ime_control_x_ = caret_rect.x;
  ime_control_y_ = caret_rect.y;
}
