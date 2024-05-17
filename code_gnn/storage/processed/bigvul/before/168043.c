bool AutofillManager::GetCachedFormAndField(const FormData& form,
                                            const FormFieldData& field,
                                            FormStructure** form_structure,
                                            AutofillField** autofill_field) {
  if (!FindCachedForm(form, form_structure) &&
      !FormStructure(form).ShouldBeParsed()) {
    return false;
  }

  if (!UpdateCachedForm(form, *form_structure, form_structure))
    return false;

  if (!(*form_structure)->autofill_count())
    return false;

  *autofill_field = nullptr;
  for (const auto& current : **form_structure) {
    if (current->SameFieldAs(field)) {
      *autofill_field = current.get();
      break;
    }
  }

  return *autofill_field != nullptr;
}
