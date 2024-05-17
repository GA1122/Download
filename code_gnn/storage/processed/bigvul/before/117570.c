SystemKeyEventListener::SystemKeyEventListener()
    : stopped_(false),
      num_lock_mask_(input_method::XKeyboard::GetNumLockMask()),
      xkb_event_base_(0) {
  input_method::XKeyboard::GetLockedModifiers(
      num_lock_mask_, &caps_lock_is_on_, &num_lock_is_on_);

  Display* display = ui::GetXDisplay();
  key_brightness_down_ = XKeysymToKeycode(display,
                                          XF86XK_MonBrightnessDown);
  key_brightness_up_ = XKeysymToKeycode(display, XF86XK_MonBrightnessUp);
  key_volume_mute_ = XKeysymToKeycode(display, XF86XK_AudioMute);
  key_volume_down_ = XKeysymToKeycode(display, XF86XK_AudioLowerVolume);
  key_volume_up_ = XKeysymToKeycode(display, XF86XK_AudioRaiseVolume);
  key_f6_ = XKeysymToKeycode(display, XK_F6);
  key_f7_ = XKeysymToKeycode(display, XK_F7);
  key_f8_ = XKeysymToKeycode(display, XK_F8);
  key_f9_ = XKeysymToKeycode(display, XK_F9);
  key_f10_ = XKeysymToKeycode(display, XK_F10);

  if (key_brightness_down_)
    GrabKey(key_brightness_down_, 0);
  if (key_brightness_up_)
    GrabKey(key_brightness_up_, 0);
  if (key_volume_mute_)
    GrabKey(key_volume_mute_, 0);
  if (key_volume_down_)
    GrabKey(key_volume_down_, 0);
  if (key_volume_up_)
    GrabKey(key_volume_up_, 0);
  GrabKey(key_f6_, 0);
  GrabKey(key_f7_, 0);
  GrabKey(key_f8_, 0);
  GrabKey(key_f9_, 0);
  GrabKey(key_f10_, 0);

  int xkb_major_version = XkbMajorVersion;
  int xkb_minor_version = XkbMinorVersion;
  if (!XkbQueryExtension(display,
                         NULL,   
                         &xkb_event_base_,
                         NULL,   
                         &xkb_major_version,
                         &xkb_minor_version)) {
    LOG(WARNING) << "Could not query Xkb extension";
  }

  if (!XkbSelectEvents(display, XkbUseCoreKbd,
                       XkbStateNotifyMask,
                       XkbStateNotifyMask)) {
    LOG(WARNING) << "Could not install Xkb Indicator observer";
  }

#if defined(TOOLKIT_USES_GTK)
  gdk_window_add_filter(NULL, GdkEventFilter, this);
#else
  MessageLoopForUI::current()->AddObserver(this);
#endif
}
