AutofillDialogViews::SuggestedButton::SuggestedButton(
    views::MenuButtonListener* listener)
    : views::MenuButton(NULL, base::string16(), listener, false) {
  const int kFocusBorderWidth = 1;
  SetBorder(views::Border::CreateEmptyBorder(kMenuButtonTopInset,
                                             kFocusBorderWidth,
                                             kMenuButtonBottomInset,
                                             kFocusBorderWidth));
  gfx::Insets insets = GetInsets();
  insets += gfx::Insets(-kFocusBorderWidth, -kFocusBorderWidth,
                        -kFocusBorderWidth, -kFocusBorderWidth);
  SetFocusPainter(
      views::Painter::CreateDashedFocusPainterWithInsets(insets));
  SetFocusable(true);
}
