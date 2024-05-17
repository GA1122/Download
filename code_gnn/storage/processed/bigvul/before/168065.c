void AutofillManager::OnFocusNoLongerOnForm() {
  ProcessPendingFormForUpload();
  if (external_delegate_->HasActiveScreenReader())
    external_delegate_->OnAutofillAvailabilityEvent(false);
}
