static bool ExecuteDelete(LocalFrame& frame,
                          Event*,
                          EditorCommandSource source,
                          const String&) {
  switch (source) {
    case kCommandFromMenuOrKeyBinding: {
      frame.GetEditor().PerformDelete();
      return true;
    }
    case kCommandFromDOM:
      DCHECK(frame.GetDocument());
      TypingCommand::DeleteKeyPressed(
          *frame.GetDocument(),
          frame.Selection().Granularity() == TextGranularity::kWord
              ? TypingCommand::kSmartDelete
              : 0);
      return true;
  }
  NOTREACHED();
  return false;
}
