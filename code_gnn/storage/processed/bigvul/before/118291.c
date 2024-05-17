int AutofillDialogViews::SuggestedButton::ResourceIDForState() const {
  views::Button::ButtonState button_state = state();
  if (button_state == views::Button::STATE_PRESSED)
    return IDR_AUTOFILL_DIALOG_MENU_BUTTON_P;
  else if (button_state == views::Button::STATE_HOVERED)
    return IDR_AUTOFILL_DIALOG_MENU_BUTTON_H;
  else if (button_state == views::Button::STATE_DISABLED)
    return IDR_AUTOFILL_DIALOG_MENU_BUTTON_D;
  DCHECK_EQ(views::Button::STATE_NORMAL, button_state);
  return IDR_AUTOFILL_DIALOG_MENU_BUTTON;
}
