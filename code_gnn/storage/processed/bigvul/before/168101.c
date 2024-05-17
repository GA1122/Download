void AutofillManager::UploadFormData(const FormStructure& submitted_form,
                                     bool observed_submission) {
  if (!download_manager_)
    return;

  bool was_autofilled = false;
  std::string form_signature = submitted_form.FormSignatureAsStr();
  for (const std::string& cur_sig : autofilled_form_signatures_) {
    if (cur_sig == form_signature) {
      was_autofilled = true;
      break;
    }
  }

  ServerFieldTypeSet non_empty_types;
  personal_data_->GetNonEmptyTypes(&non_empty_types);
  if (submitted_form.is_signin_upload())
    non_empty_types.insert(PASSWORD);

  download_manager_->StartUploadRequest(
      submitted_form, was_autofilled, non_empty_types,
       std::string(), observed_submission);
}
