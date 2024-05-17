bool SystemKeyEventListener::ProcessedXEvent(XEvent* xevent) {
  input_method::InputMethodManager* input_method_manager =
      input_method::InputMethodManager::GetInstance();

#if !defined(USE_AURA)
  if (xevent->type == FocusIn) {
    input_method::HotkeyManager* hotkey_manager =
        input_method_manager->GetHotkeyManager();
    hotkey_manager->OnFocus();
  }
#endif

  if (xevent->type == KeyPress || xevent->type == KeyRelease) {
    input_method::HotkeyManager* hotkey_manager =
        input_method_manager->GetHotkeyManager();
    if (hotkey_manager->FilterKeyEvent(*xevent))
      return true;
  }

  if (xevent->type == xkb_event_base_) {
    XkbEvent* xkey_event = reinterpret_cast<XkbEvent*>(xevent);
    if (xkey_event->any.xkb_type == XkbStateNotify) {
      input_method::ModifierLockStatus new_caps_lock_state =
          input_method::kDontChange;
      input_method::ModifierLockStatus new_num_lock_state =
          input_method::kDontChange;

      bool enabled = (xkey_event->state.locked_mods) & LockMask;
      if (caps_lock_is_on_ != enabled) {
        caps_lock_is_on_ = enabled;
        new_caps_lock_state =
            enabled ? input_method::kEnableLock : input_method::kDisableLock;
        OnCapsLock(caps_lock_is_on_);
      }

      enabled = (xkey_event->state.locked_mods) & num_lock_mask_;
      if (num_lock_is_on_ != enabled) {
        num_lock_is_on_ = enabled;
        new_num_lock_state =
            enabled ? input_method::kEnableLock : input_method::kDisableLock;
      }

      input_method_manager->GetXKeyboard()->SetLockedModifiers(
          new_caps_lock_state, new_num_lock_state);

      return true;
    }
  } else if (xevent->type == KeyPress) {
    const int32 keycode = xevent->xkey.keycode;
    if (keycode) {
      const unsigned int state = (xevent->xkey.state & kSupportedModifiers);

      if (XKeycodeToKeysym(ui::GetXDisplay(), keycode, 0) == XK_Super_L) {
        const bool shift_is_held = (state & ShiftMask);
        const bool other_mods_are_held = (state & ~(ShiftMask | LockMask));
        if (shift_is_held && !other_mods_are_held)
          input_method_manager->GetXKeyboard()->SetCapsLockEnabled(
              !caps_lock_is_on_);
      }

      if (!(state & (Mod1Mask | ShiftMask | ControlMask))) {
        if (keycode == key_f6_ || keycode == key_brightness_down_) {
          if (keycode == key_f6_)
            content::RecordAction(
                UserMetricsAction("Accel_BrightnessDown_F6"));
          OnBrightnessDown();
          return true;
        } else if (keycode == key_f7_ || keycode == key_brightness_up_) {
          if (keycode == key_f7_)
            content::RecordAction(
                UserMetricsAction("Accel_BrightnessUp_F7"));
          OnBrightnessUp();
          return true;
        } else if (keycode == key_f8_ || keycode == key_volume_mute_) {
          if (keycode == key_f8_)
            content::RecordAction(UserMetricsAction("Accel_VolumeMute_F8"));
          OnVolumeMute();
          return true;
        } else if (keycode == key_f9_ || keycode == key_volume_down_) {
          if (keycode == key_f9_)
            content::RecordAction(UserMetricsAction("Accel_VolumeDown_F9"));
          OnVolumeDown();
          return true;
        } else if (keycode == key_f10_ || keycode == key_volume_up_) {
          if (keycode == key_f10_)
            content::RecordAction(UserMetricsAction("Accel_VolumeUp_F10"));
          OnVolumeUp();
          return true;
        }
      }
    }
  }
  return false;
}
