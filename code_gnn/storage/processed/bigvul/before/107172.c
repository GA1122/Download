bool FormIsAutoFilled(const FormStructure* form_structure,
                      const webkit_glue::FormData& form,
                      bool is_filling_credit_card) {

  for (size_t i = 0, j = 0;
       i < form_structure->field_count() && j < form.fields.size();
       j++) {
    size_t k = i;

    while (k < form_structure->field_count() &&
           *form_structure->field(k) != form.fields[j]) {
      k++;
    }

    if (k >= form_structure->field_count())
      continue;

    AutoFillType autofill_type(form_structure->field(k)->type());
    bool is_credit_card_field =
        autofill_type.group() == AutoFillType::CREDIT_CARD;
    if (is_filling_credit_card == is_credit_card_field &&
        form.fields[j].is_autofilled())
      return true;

    ++i;
  }

  return false;
}
