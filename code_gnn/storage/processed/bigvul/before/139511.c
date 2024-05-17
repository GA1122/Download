static bool ExecuteStyleWithCSS(LocalFrame& frame,
                                Event*,
                                EditorCommandSource,
                                const String& value) {
  frame.GetEditor().SetShouldStyleWithCSS(
      !DeprecatedEqualIgnoringCase(value, "false"));
  return true;
}
