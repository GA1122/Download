static bool ApplyCommandToFrame(LocalFrame& frame,
                                EditorCommandSource source,
                                InputEvent::InputType input_type,
                                StylePropertySet* style) {
  switch (source) {
    case kCommandFromMenuOrKeyBinding:
      frame.GetEditor().ApplyStyleToSelection(style, input_type);
      return true;
    case kCommandFromDOM:
      frame.GetEditor().ApplyStyle(style, input_type);
      return true;
  }
  NOTREACHED();
  return false;
}
