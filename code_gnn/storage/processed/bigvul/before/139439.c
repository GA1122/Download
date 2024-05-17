static bool ExecuteInsertText(LocalFrame& frame,
                              Event*,
                              EditorCommandSource,
                              const String& value) {
  DCHECK(frame.GetDocument());
  TypingCommand::InsertText(*frame.GetDocument(), value, 0);
  return true;
}
