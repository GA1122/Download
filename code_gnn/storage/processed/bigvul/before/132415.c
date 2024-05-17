void MarkKeyboardLoadFinished() {
  if (!g_keyboard_load_time_start.Get().ToInternalValue())
    return;

  DCHECK(g_keyboard_load_time_start.Get().ToInternalValue());

  static bool logged = false;
  if (!logged) {
    UMA_HISTOGRAM_TIMES(
        "VirtualKeyboard.FirstLoadTime",
        base::Time::Now() - g_keyboard_load_time_start.Get());
    logged = true;
  }
}
