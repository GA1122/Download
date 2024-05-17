void AutoFillManager::UploadFormData(const FormStructure& submitted_form) {
  if (!disable_download_manager_requests_) {
    bool was_autofilled = false;
    std::list<std::string>::iterator it;
    int total_form_checked = 0;
    for (it = autofilled_forms_signatures_.begin();
         it != autofilled_forms_signatures_.end() && total_form_checked < 3;
         ++it, ++total_form_checked) {
      if (*it == submitted_form.FormSignature())
        was_autofilled = true;
    }
    if (total_form_checked == 3 && it != autofilled_forms_signatures_.end()) {
      autofilled_forms_signatures_.erase(it,
                                         autofilled_forms_signatures_.end());
    }
    download_manager_.StartUploadRequest(submitted_form, was_autofilled);
  }
}
