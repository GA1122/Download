void AutofillManager::ProcessPendingFormForUpload() {
  if (!pending_form_data_)
    return;

  std::unique_ptr<FormStructure> upload_form =
      ValidateSubmittedForm(*pending_form_data_);
  pending_form_data_.reset();
  if (!upload_form)
    return;

  MaybeStartVoteUploadProcess(std::move(upload_form), TimeTicks::Now(),
                               false);
}
