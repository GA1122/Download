void InputHandler::ClearInputState() {
  for (auto& callback : pending_key_callbacks_)
    callback->sendSuccess();
  pending_key_callbacks_.clear();
  for (auto& callback : pending_mouse_callbacks_)
    callback->sendSuccess();
  pending_mouse_callbacks_.clear();
  touch_points_.clear();
}
