static bool ExecuteInsertTab(LocalFrame& frame,
                             Event* event,
                             EditorCommandSource,
                             const String&) {
  return TargetFrame(frame, event)
      ->GetEventHandler()
      .HandleTextInputEvent("\t", event);
}
