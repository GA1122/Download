void GamepadProvider::DoPoll() {
  DCHECK(polling_thread_->task_runner()->BelongsToCurrentThread());
  DCHECK(have_scheduled_do_poll_);
  have_scheduled_do_poll_ = false;

  bool changed;

  ANNOTATE_BENIGN_RACE_SIZED(gamepad_shared_buffer_->buffer(), sizeof(Gamepads),
                             "Racey reads are discarded");

  {
    base::AutoLock lock(devices_changed_lock_);
    changed = devices_changed_;
    devices_changed_ = false;
  }

  for (const auto& it : data_fetchers_) {
    it->GetGamepadData(changed);
  }

  Gamepads* buffer = gamepad_shared_buffer_->buffer();

  if (ever_had_user_gesture_) {
    for (unsigned i = 0; i < Gamepads::kItemsLengthCap; ++i) {
      PadState& state = pad_states_.get()[i];

      if (!state.active_state && state.source != GAMEPAD_SOURCE_NONE) {
        auto pad = buffer->items[i];
        pad.connected = false;
        OnGamepadConnectionChange(false, i, pad);
        ClearPadState(state);
      }
    }
  }

  {
    base::AutoLock lock(shared_memory_lock_);

    gamepad_shared_buffer_->WriteBegin();
    for (unsigned i = 0; i < Gamepads::kItemsLengthCap; ++i) {
      PadState& state = pad_states_.get()[i];
      MapAndSanitizeGamepadData(&state, &buffer->items[i], sanitize_);
    }
    gamepad_shared_buffer_->WriteEnd();
  }

  if (ever_had_user_gesture_) {
    for (unsigned i = 0; i < Gamepads::kItemsLengthCap; ++i) {
      PadState& state = pad_states_.get()[i];

      if (state.active_state) {
        if (state.active_state == GAMEPAD_NEWLY_ACTIVE &&
            buffer->items[i].connected) {
          OnGamepadConnectionChange(true, i, buffer->items[i]);
        }
      }
    }
  }

  CheckForUserGesture();

  if (ever_had_user_gesture_) {
    for (unsigned i = 0; i < Gamepads::kItemsLengthCap; ++i)
      pad_states_.get()[i].active_state = GAMEPAD_INACTIVE;
  }

  ScheduleDoPoll();
}
