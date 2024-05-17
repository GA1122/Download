static bool ExecuteOutdent(LocalFrame& frame,
                           Event*,
                           EditorCommandSource,
                           const String&) {
  DCHECK(frame.GetDocument());
  return IndentOutdentCommand::Create(*frame.GetDocument(),
                                      IndentOutdentCommand::kOutdent)
      ->Apply();
}
