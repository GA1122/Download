void AutofillManager::UploadFormData(const FormStructure& submitted_form) {
  if (disable_download_manager_requests_)
    return;

  bool was_autofilled = false;
  std::string form_signature = submitted_form.FormSignature();
  for (std::list<std::string>::const_iterator it =
           autofilled_form_signatures_.begin();
       it != autofilled_form_signatures_.end() && !was_autofilled;
       ++it) {
    if (*it == form_signature)
      was_autofilled = true;
  }

  FieldTypeSet non_empty_types;
  personal_data_->GetNonEmptyTypes(&non_empty_types);

  download_manager_.StartUploadRequest(submitted_form, was_autofilled,
                                       non_empty_types);
}
