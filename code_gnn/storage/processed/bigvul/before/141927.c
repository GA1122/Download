PopupItemLayoutType AutofillPopupFooterView::GetLayoutType() const {
  return frontend_id() ==
                 autofill::PopupItemId::POPUP_ITEM_ID_SHOW_ACCOUNT_CARDS
             ? PopupItemLayoutType::kLeadingIcon
             : PopupItemLayoutType::kTrailingIcon;
}
