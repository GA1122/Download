std::unique_ptr<FormStructure> AutofillManager::ValidateSubmittedForm(
    const FormData& form) {
  std::unique_ptr<FormStructure> submitted_form(
      std::make_unique<FormStructure>(form));
  if (!ShouldUploadForm(*submitted_form))
    return std::unique_ptr<FormStructure>();

  FormStructure* cached_submitted_form;
  if (!FindCachedForm(form, &cached_submitted_form))
    return std::unique_ptr<FormStructure>();

  submitted_form->RetrieveFromCache(*cached_submitted_form,
                                     false,
                                     false);
  return submitted_form;
}
