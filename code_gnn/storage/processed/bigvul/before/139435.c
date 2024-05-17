static bool ExecuteInsertNewlineInQuotedContent(LocalFrame& frame,
                                                Event*,
                                                EditorCommandSource,
                                                const String&) {
  DCHECK(frame.GetDocument());
  return TypingCommand::InsertParagraphSeparatorInQuotedContent(
      *frame.GetDocument());
}
