void AutofillManager::OnFormSubmitted(const FormData& form) {
  tab_contents_wrapper_->autocomplete_history_manager()->OnFormSubmitted(form);

  if (!IsAutofillEnabled())
    return;

  if (tab_contents()->profile()->IsOffTheRecord())
    return;

  if (!form.user_submitted)
    return;

  FormStructure submitted_form(form);

  if (!submitted_form.ShouldBeParsed(true))
    return;

  FormStructure* cached_submitted_form;
  if (!FindCachedForm(form, &cached_submitted_form))
    return;
  submitted_form.UpdateFromCache(*cached_submitted_form);

  DeterminePossibleFieldTypesForUpload(&submitted_form);
  UploadFormData(submitted_form);

  submitted_form.LogQualityMetrics(*metric_logger_);

  if (!submitted_form.IsAutofillable(true))
    return;

  ImportFormData(submitted_form);
}
