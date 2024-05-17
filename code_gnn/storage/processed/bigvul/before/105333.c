void FindSectionBounds(const FormStructure& form,
                       const AutofillField& field,
                       bool is_filling_credit_card,
                       size_t* section_start,
                       size_t* section_end) {
  DCHECK(section_start);
  DCHECK(section_end);

  *section_start = 0;
  *section_end = form.field_count();

  std::set<AutofillFieldType> seen_types;
  bool initiating_field_is_in_current_section = false;
  for (size_t i = 0; i < form.field_count(); ++i) {
    const AutofillField* current_field = form.field(i);
    const AutofillFieldType current_type =
        AutofillType::GetEquivalentFieldType(current_field->type());

    if (current_type == UNKNOWN_TYPE)
      continue;

    bool already_saw_current_type = seen_types.count(current_type) > 0;
    AutofillType::FieldTypeGroup current_type_group =
        AutofillType(current_type).group();
    if (current_type_group == AutofillType::PHONE_HOME ||
        current_type_group == AutofillType::PHONE_FAX ||
        current_type_group == AutofillType::EMAIL)
      already_saw_current_type = false;

    bool is_credit_card_field = current_type_group == AutofillType::CREDIT_CARD;
    bool is_appropriate_type = is_credit_card_field == is_filling_credit_card;

    if (already_saw_current_type || !is_appropriate_type) {
      if (initiating_field_is_in_current_section) {
        *section_end = i;
        break;
      }

      seen_types.clear();

      if (is_appropriate_type) {
        *section_start = i;
      } else {
        *section_start = i + 1;
        continue;
      }
    }

    seen_types.insert(current_type);

    if (current_field == &field)
      initiating_field_is_in_current_section = true;
  }

  DCHECK(initiating_field_is_in_current_section);
}
