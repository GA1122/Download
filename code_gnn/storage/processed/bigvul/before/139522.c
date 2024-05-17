static bool ExecuteUnlink(LocalFrame& frame,
                          Event*,
                          EditorCommandSource,
                          const String&) {
  DCHECK(frame.GetDocument());
  return UnlinkCommand::Create(*frame.GetDocument())->Apply();
}
