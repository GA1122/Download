 void GamepadProvider::GetCurrentGamepadData(Gamepads* data) {
  const Gamepads* pads = gamepad_shared_buffer_->buffer();
  base::AutoLock lock(shared_memory_lock_);
  *data = *pads;
}
