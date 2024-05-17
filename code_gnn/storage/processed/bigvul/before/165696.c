void Trace(const wchar_t* format_string, ...) {
  static const int kMaxLogBufferSize = 1024;
  static wchar_t buffer[kMaxLogBufferSize] = {};

  va_list args = {};

  va_start(args, format_string);
  vswprintf(buffer, kMaxLogBufferSize, format_string, args);
  OutputDebugStringW(buffer);
  va_end(args);
}
