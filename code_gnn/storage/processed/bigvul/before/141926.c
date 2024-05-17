PopupItemLayoutType AutofillPopupSuggestionView::GetLayoutType() const {
  switch (GetForcedPopupLayoutState()) {
    case ForcedPopupLayoutState::kLeadingIcon:
      return PopupItemLayoutType::kLeadingIcon;
    case ForcedPopupLayoutState::kTrailingIcon:
      return PopupItemLayoutType::kTrailingIcon;
    case ForcedPopupLayoutState::kTwoLinesLeadingIcon:
      return PopupItemLayoutType::kTwoLinesLeadingIcon;
    case ForcedPopupLayoutState::kDefault:
      switch (frontend_id()) {
        case autofill::PopupItemId::POPUP_ITEM_ID_USERNAME_ENTRY:
        case autofill::PopupItemId::POPUP_ITEM_ID_PASSWORD_ENTRY:
        case autofill::PopupItemId::POPUP_ITEM_ID_GENERATE_PASSWORD_ENTRY:
          return PopupItemLayoutType::kLeadingIcon;
        default:
          return PopupItemLayoutType::kTrailingIcon;
      }
  }
}
