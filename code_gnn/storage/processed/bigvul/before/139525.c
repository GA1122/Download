static bool ExecuteUseCSS(LocalFrame& frame,
                          Event*,
                          EditorCommandSource,
                          const String& value) {
  frame.GetEditor().SetShouldStyleWithCSS(
      DeprecatedEqualIgnoringCase(value, "false"));
  return true;
}
