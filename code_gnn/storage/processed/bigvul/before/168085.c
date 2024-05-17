void AutofillManager::SelectFieldOptionsDidChange(const FormData& form) {
  FormStructure* form_structure = nullptr;

  FormStructure* cached_form = nullptr;
  ignore_result(FindCachedForm(form, &cached_form));

  if (!ParseFormInternal(form, cached_form, &form_structure))
    return;

  if (ShouldTriggerRefill(*form_structure))
    TriggerRefill(form, form_structure);
}
