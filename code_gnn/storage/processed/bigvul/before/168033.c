void AutofillManager::DisambiguatePhoneUploadTypes(FormStructure* form,
                                                   size_t current_index) {

  ServerFieldTypeSet matching_types;
  matching_types.insert(PHONE_HOME_CITY_AND_NUMBER);
  AutofillField* field = form->field(current_index);
  field->set_possible_types(matching_types);
}
