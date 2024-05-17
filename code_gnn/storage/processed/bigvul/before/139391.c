static bool EnabledRedo(LocalFrame& frame, Event*, EditorCommandSource) {
  return frame.GetEditor().CanRedo();
}
