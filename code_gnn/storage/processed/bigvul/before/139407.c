static bool ExecuteDefaultParagraphSeparator(LocalFrame& frame,
                                             Event*,
                                             EditorCommandSource,
                                             const String& value) {
  if (DeprecatedEqualIgnoringCase(value, "div"))
    frame.GetEditor().SetDefaultParagraphSeparator(
        kEditorParagraphSeparatorIsDiv);
  else if (DeprecatedEqualIgnoringCase(value, "p"))
    frame.GetEditor().SetDefaultParagraphSeparator(
        kEditorParagraphSeparatorIsP);

  return true;
}
