void AutoFillManager::OnFormSubmitted(const FormData& form) {
  tab_contents_->autocomplete_history_manager()->OnFormSubmitted(form);

  if (!IsAutoFillEnabled())
    return;

  if (tab_contents_->profile()->IsOffTheRecord())
    return;

  if (!form.user_submitted)
    return;

  FormStructure submitted_form(form);

  if (!submitted_form.ShouldBeParsed(true))
    return;

  DeterminePossibleFieldTypesForUpload(&submitted_form);
  LogMetricsAboutSubmittedForm(form, &submitted_form);

  UploadFormData(submitted_form);

  if (!submitted_form.IsAutoFillable(true))
    return;

  ImportFormData(submitted_form);
}
