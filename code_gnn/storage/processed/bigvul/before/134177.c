InputMethodWin::InputMethodWin(internal::InputMethodDelegate* delegate,
                               HWND toplevel_window_handle)
    : active_(false),
      toplevel_window_handle_(toplevel_window_handle),
      direction_(base::i18n::UNKNOWN_DIRECTION),
      pending_requested_direction_(base::i18n::UNKNOWN_DIRECTION) {
  SetDelegate(delegate);
}
