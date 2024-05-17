static bool ExecuteInsertUnorderedList(LocalFrame& frame,
                                       Event*,
                                       EditorCommandSource,
                                       const String&) {
  DCHECK(frame.GetDocument());
  return InsertListCommand::Create(*frame.GetDocument(),
                                   InsertListCommand::kUnorderedList)
      ->Apply();
}
