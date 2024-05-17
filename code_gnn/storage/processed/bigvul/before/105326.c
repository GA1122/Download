void AutofillManager::DeterminePossibleFieldTypesForUpload(
    FormStructure* submitted_form) {
  for (size_t i = 0; i < submitted_form->field_count(); i++) {
    const AutofillField* field = submitted_form->field(i);
    FieldTypeSet field_types;
    personal_data_->GetMatchingTypes(field->value, &field_types);

    DCHECK(!field_types.empty());
    submitted_form->set_possible_types(i, field_types);
  }
}
