void AutoFillManager::GetProfileSuggestions(FormStructure* form,
                                            const FormField& field,
                                            AutoFillType type,
                                            std::vector<string16>* values,
                                            std::vector<string16>* labels,
                                            std::vector<string16>* icons,
                                            std::vector<int>* unique_ids) {
  const std::vector<AutoFillProfile*>& profiles = personal_data_->profiles();
  std::vector<AutoFillProfile*> matched_profiles;
  for (std::vector<AutoFillProfile*>::const_iterator iter = profiles.begin();
       iter != profiles.end(); ++iter) {
    AutoFillProfile* profile = *iter;

    string16 profile_field_value = profile->GetFieldText(type);

    if (!profile_field_value.empty() &&
        StartsWith(profile_field_value, field.value(), false)) {
      matched_profiles.push_back(profile);
      values->push_back(profile_field_value);
      unique_ids->push_back(PackGUIDs(std::string(), profile->guid()));
    }
  }

  std::vector<AutoFillFieldType> form_fields;
  form_fields.reserve(form->field_count());
  for (std::vector<AutoFillField*>::const_iterator iter = form->begin();
       iter != form->end(); ++iter) {
    if (!*iter)
      break;
    form_fields.push_back((*iter)->type());
  }

  AutoFillProfile::CreateInferredLabels(&matched_profiles, &form_fields,
                                        type.field_type(), 1, labels);

  icons->resize(values->size());
}
