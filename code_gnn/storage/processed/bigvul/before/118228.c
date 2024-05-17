int AutofillDialogViews::SuggestionView::GetHeightForWidth(int width) const {
  int height = 0;
  CanUseVerticallyCompactText(width, &height);
  return height;
}
