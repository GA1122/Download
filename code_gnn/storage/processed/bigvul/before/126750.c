void BrowserView::GetAccessibleState(ui::AccessibleViewState* state) {
  state->name = l10n_util::GetStringUTF16(IDS_PRODUCT_NAME);
  state->role = ui::AccessibilityTypes::ROLE_CLIENT;
}
