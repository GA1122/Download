bool AutoFillManager::FindCachedFormAndField(const FormData& form,
                                             const FormField& field,
                                             FormStructure** form_structure,
                                             AutoFillField** autofill_field) {
  if (!FindCachedForm(form, form_structure))
    return false;

  if (!(*form_structure)->autofill_count())
    return false;

  *autofill_field = NULL;
  for (std::vector<AutoFillField*>::const_iterator iter =
           (*form_structure)->begin();
       iter != (*form_structure)->end(); ++iter) {
    if (!*iter)
      break;

    if ((**iter) == field) {
      *autofill_field = *iter;
      break;
    }
  }

  if (!(*autofill_field))
    return false;

  return true;
}
