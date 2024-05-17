void Editor::applyStyle(StylePropertySet* style,
                        InputEvent::InputType inputType) {
  switch (frame()
              .selection()
              .computeVisibleSelectionInDOMTreeDeprecated()
              .getSelectionType()) {
    case NoSelection:
      break;
    case CaretSelection:
      computeAndSetTypingStyle(style, inputType);
      break;
    case RangeSelection:
      if (style) {
        DCHECK(frame().document());
        ApplyStyleCommand::create(*frame().document(),
                                  EditingStyle::create(style), inputType)
            ->apply();
      }
      break;
  }
}
