void Editor::ApplyStyle(StylePropertySet* style,
                        InputEvent::InputType input_type) {
  switch (GetFrame()
              .Selection()
              .ComputeVisibleSelectionInDOMTreeDeprecated()
              .GetSelectionType()) {
    case kNoSelection:
      break;
    case kCaretSelection:
      ComputeAndSetTypingStyle(style, input_type);
      break;
    case kRangeSelection:
      if (style) {
        DCHECK(GetFrame().GetDocument());
        ApplyStyleCommand::Create(*GetFrame().GetDocument(),
                                  EditingStyle::Create(style), input_type)
            ->Apply();
      }
      break;
  }
}
