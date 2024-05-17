static bool ExecuteIgnoreSpelling(LocalFrame& frame,
                                  Event*,
                                  EditorCommandSource,
                                  const String&) {
  frame.GetSpellChecker().IgnoreSpelling();
  return true;
}
