void ManualFillingControllerImpl::OnFilledIntoFocusedField(
    autofill::FillingStatus status) {
  if (status != autofill::FillingStatus::SUCCESS)
    return;   
  view_->SwapSheetWithKeyboard();
}
