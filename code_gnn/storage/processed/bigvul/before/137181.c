base::TimeDelta Textfield::GetCaretBlinkInterval() {
  static constexpr base::TimeDelta default_value =
      base::TimeDelta::FromMilliseconds(500);
#if defined(OS_WIN)
  static const size_t system_value = ::GetCaretBlinkTime();
  if (system_value != 0) {
    return (system_value == INFINITE)
               ? base::TimeDelta()
               : base::TimeDelta::FromMilliseconds(system_value);
  }
#endif
  return default_value;
}
