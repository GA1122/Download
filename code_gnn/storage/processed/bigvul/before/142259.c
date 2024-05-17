void ManualFillingControllerImpl::RefreshSuggestionsForField(
    bool is_fillable,
    const AccessorySheetData& accessory_sheet_data) {
  view_->OnItemsAvailable(accessory_sheet_data);

  if (is_fillable) {
    view_->SwapSheetWithKeyboard();
  } else {
    view_->CloseAccessorySheet();
  }
}
