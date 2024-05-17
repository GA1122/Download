static bool ExecuteInsertBacktab(LocalFrame& frame,
                                 Event* event,
                                 EditorCommandSource,
                                 const String&) {
  return TargetFrame(frame, event)
      ->GetEventHandler()
      .HandleTextInputEvent("\t", event);
}
