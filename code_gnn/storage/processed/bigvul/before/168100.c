void AutofillManager::UpdatePendingForm(const FormData& form) {
  if (pending_form_data_ && !pending_form_data_->SameFormAs(form)) {
    ProcessPendingFormForUpload();
  }
  pending_form_data_.reset(new FormData(form));
}
