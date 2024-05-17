static bool ExecuteFindString(LocalFrame& frame,
                              Event*,
                              EditorCommandSource,
                              const String& value) {
  return frame.GetEditor().FindString(value, kCaseInsensitive | kWrapAround);
}
