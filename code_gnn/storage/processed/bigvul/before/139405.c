static bool ExecuteCreateLink(LocalFrame& frame,
                              Event*,
                              EditorCommandSource,
                              const String& value) {
  if (value.IsEmpty())
    return false;
  DCHECK(frame.GetDocument());
  return CreateLinkCommand::Create(*frame.GetDocument(), value)->Apply();
}
