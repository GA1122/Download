void Editor::computeAndSetTypingStyle(StylePropertySet* style,
                                      InputEvent::InputType inputType) {
  if (!style || style->isEmpty()) {
    clearTypingStyle();
    return;
  }

  if (m_typingStyle)
    m_typingStyle->overrideWithStyle(style);
  else
    m_typingStyle = EditingStyle::create(style);

  m_typingStyle->prepareToApplyAt(
      frame()
          .selection()
          .computeVisibleSelectionInDOMTreeDeprecated()
          .visibleStart()
          .deepEquivalent(),
      EditingStyle::PreserveWritingDirection);

  EditingStyle* blockStyle = m_typingStyle->extractAndRemoveBlockProperties();
  if (!blockStyle->isEmpty()) {
    DCHECK(frame().document());
    ApplyStyleCommand::create(*frame().document(), blockStyle, inputType)
        ->apply();
  }
}
