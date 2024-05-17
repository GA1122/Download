static bool ExecuteYank(LocalFrame& frame,
                        Event*,
                        EditorCommandSource,
                        const String&) {
  const String& yank_string = frame.GetEditor().GetKillRing().Yank();
  if (DispatchBeforeInputInsertText(
          EventTargetNodeForDocument(frame.GetDocument()), yank_string,
          InputEvent::InputType::kInsertFromYank) !=
      DispatchEventResult::kNotCanceled)
    return true;

  if (frame.GetDocument()->GetFrame() != &frame)
    return false;

  frame.GetDocument()->UpdateStyleAndLayoutIgnorePendingStylesheets();

  frame.GetEditor().InsertTextWithoutSendingTextEvent(
      yank_string, false, 0, InputEvent::InputType::kInsertFromYank);
  frame.GetEditor().GetKillRing().SetToYankedState();
  return true;
}
