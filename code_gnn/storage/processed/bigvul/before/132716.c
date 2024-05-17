void ChromotingInstance::HandleAllowMouseLockMessage() {
  mouse_locker_.reset(new PepperMouseLocker(
      this, base::Bind(&PepperInputHandler::set_send_mouse_move_deltas,
                       base::Unretained(&input_handler_)),
      &cursor_setter_));
  empty_cursor_filter_.set_cursor_stub(mouse_locker_.get());
}
