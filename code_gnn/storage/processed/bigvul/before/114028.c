void DispatchKeyReleaseA() {
#if defined(OS_WIN)
  MSG native_event_down = { NULL, WM_KEYDOWN, ui::VKEY_A, 0 };
  ash::Shell::GetRootWindow()->PostNativeEvent(native_event_down);
  MSG native_event_up = { NULL, WM_KEYUP, ui::VKEY_A, 0 };
  ash::Shell::GetRootWindow()->PostNativeEvent(native_event_up);
#elif defined(USE_X11)
  XEvent native_event;
  ui::InitXKeyEventForTesting(ui::ET_KEY_PRESSED,
                              ui::VKEY_A,
                              0,
                              &native_event);
  ash::Shell::GetRootWindow()->PostNativeEvent(&native_event);
  ui::InitXKeyEventForTesting(ui::ET_KEY_RELEASED,
                              ui::VKEY_A,
                              0,
                              &native_event);
  ash::Shell::GetRootWindow()->PostNativeEvent(&native_event);
#endif

  ash::Shell::GetRootWindow()->PostNativeEvent(ui::CreateNoopEvent());
}
