static bool ExecuteInsertFragment(LocalFrame& frame,
                                  DocumentFragment* fragment) {
  DCHECK(frame.GetDocument());
  return ReplaceSelectionCommand::Create(
             *frame.GetDocument(), fragment,
             ReplaceSelectionCommand::kPreventNesting,
             InputEvent::InputType::kNone)
      ->Apply();
}
