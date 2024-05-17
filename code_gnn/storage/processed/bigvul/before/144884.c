bool RenderWidgetHostViewAura::ChangeTextDirectionAndLayoutAlignment(
      base::i18n::TextDirection direction) {
  if (!GetTextInputManager() && !GetTextInputManager()->GetActiveWidget())
    return false;

  GetTextInputManager()->GetActiveWidget()->UpdateTextDirection(
      direction == base::i18n::RIGHT_TO_LEFT
          ? blink::kWebTextDirectionRightToLeft
          : blink::kWebTextDirectionLeftToRight);
  GetTextInputManager()->GetActiveWidget()->NotifyTextDirection();
  return true;
}
