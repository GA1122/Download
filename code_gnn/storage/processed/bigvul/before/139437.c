static bool ExecuteInsertParagraph(LocalFrame& frame,
                                   Event*,
                                   EditorCommandSource,
                                   const String&) {
  DCHECK(frame.GetDocument());
  return TypingCommand::InsertParagraphSeparator(*frame.GetDocument());
}
