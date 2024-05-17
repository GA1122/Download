static bool ExecuteInsertHTML(LocalFrame& frame,
                              Event*,
                              EditorCommandSource,
                              const String& value) {
  DCHECK(frame.GetDocument());
  return ExecuteInsertFragment(
      frame, CreateFragmentFromMarkup(*frame.GetDocument(), value, ""));
}
