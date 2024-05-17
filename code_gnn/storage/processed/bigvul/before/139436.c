static bool ExecuteInsertOrderedList(LocalFrame& frame,
                                     Event*,
                                     EditorCommandSource,
                                     const String&) {
  DCHECK(frame.GetDocument());
  return InsertListCommand::Create(*frame.GetDocument(),
                                   InsertListCommand::kOrderedList)
      ->Apply();
}
