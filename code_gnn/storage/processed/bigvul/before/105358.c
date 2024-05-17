bool SectionIsAutofilled(const FormStructure* form_structure,
                         const webkit_glue::FormData& form,
                         size_t section_start,
                         size_t section_end) {

  for (size_t i = section_start, j = 0;
       i < section_end && j < form.fields.size();
       j++) {
    size_t k = i;

    while (k < form_structure->field_count() &&
           *form_structure->field(k) != form.fields[j]) {
      k++;
    }

    if (k >= form_structure->field_count())
      continue;

    AutofillType autofill_type(form_structure->field(k)->type());
    if (form.fields[j].is_autofilled)
      return true;

    ++i;
  }

  return false;
}
