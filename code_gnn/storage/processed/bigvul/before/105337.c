void AutofillManager::GetProfileSuggestions(FormStructure* form,
                                            const FormField& field,
                                            AutofillFieldType type,
                                            std::vector<string16>* values,
                                            std::vector<string16>* labels,
                                            std::vector<string16>* icons,
                                            std::vector<int>* unique_ids) {
  const std::vector<AutofillProfile*>& profiles = personal_data_->profiles();
  if (!field.is_autofilled) {
    std::vector<AutofillProfile*> matched_profiles;
    for (std::vector<AutofillProfile*>::const_iterator iter = profiles.begin();
         iter != profiles.end(); ++iter) {
      AutofillProfile* profile = *iter;

      std::vector<string16> multi_values;
      profile->GetMultiInfo(type, &multi_values);
      NormalizePhoneMultiInfo(type, profile->CountryCode(), &multi_values);

      for (size_t i = 0; i < multi_values.size(); ++i) {
        if (!multi_values[i].empty() &&
            StartsWith(multi_values[i], field.value, false)) {
          matched_profiles.push_back(profile);
          values->push_back(multi_values[i]);
          unique_ids->push_back(PackGUIDs(GUIDPair(std::string(), 0),
                                          GUIDPair(profile->guid(), i)));
          break;
        }
      }
    }

    std::vector<AutofillFieldType> form_fields;
    form_fields.reserve(form->field_count());
    for (std::vector<AutofillField*>::const_iterator iter = form->begin();
         iter != form->end(); ++iter) {
      form_fields.push_back((*iter)->type());
    }

    AutofillProfile::CreateInferredLabels(&matched_profiles, &form_fields,
                                          type, 1, labels);

    icons->resize(values->size());
  } else {
    for (std::vector<AutofillProfile*>::const_iterator iter = profiles.begin();
         iter != profiles.end(); ++iter) {
      AutofillProfile* profile = *iter;

      std::vector<string16> multi_values;
      profile->GetMultiInfo(type, &multi_values);
      NormalizePhoneMultiInfo(type, profile->CountryCode(), &multi_values);

      for (size_t i = 0; i < multi_values.size(); ++i) {
        if (multi_values[i].empty())
          continue;
        string16 profile_value_lower_case(StringToLowerASCII(multi_values[i]));
        string16 field_value_lower_case(StringToLowerASCII(field.value));
        bool matched_phones = false;
        if ((type == PHONE_HOME_NUMBER || type == PHONE_FAX_NUMBER) &&
            !field_value_lower_case.empty() &&
            (profile_value_lower_case.find(field_value_lower_case) !=
             string16::npos)) {
          matched_phones = true;
        }
        if (matched_phones ||
            profile_value_lower_case == field_value_lower_case) {
          for (size_t j = 0; j < multi_values.size(); ++j) {
            if (!multi_values[j].empty()) {
              values->push_back(multi_values[j]);
              unique_ids->push_back(PackGUIDs(GUIDPair(std::string(), 0),
                                              GUIDPair(profile->guid(), j)));
            }
          }
          break;
        }
      }
    }

    labels->resize(values->size());

    icons->resize(values->size());
  }
}
