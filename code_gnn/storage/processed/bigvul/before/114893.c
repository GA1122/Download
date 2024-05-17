TestingAutomationProvider::GetAutofillProfilesFromList(
    const ListValue& profiles, std::string* error_message) {
  std::vector<AutofillProfile> autofill_profiles;
  DictionaryValue* profile_info = NULL;
  ListValue* current_value = NULL;

  std::map<AutofillFieldType, std::string> autofill_type_to_string =
      GetAutofillFieldToStringMap();

  int num_profiles = profiles.GetSize();
  for (int i = 0; i < num_profiles; i++) {
    profiles.GetDictionary(i, &profile_info);
    AutofillProfile profile;
    for (std::map<AutofillFieldType, std::string>::iterator type_it =
         autofill_type_to_string.begin();
         type_it != autofill_type_to_string.end(); ++type_it) {
      if (profile_info->HasKey(type_it->second)) {
        if (profile_info->GetList(type_it->second, &current_value)) {
          std::vector<string16> value_list;
          for (ListValue::iterator list_it = current_value->begin();
               list_it != current_value->end(); ++list_it) {
            string16 value;
            if ((*list_it)->GetAsString(&value)) {
              value_list.insert(value_list.end(), value);
            } else {
              *error_message = "All list values must be strings";
              return autofill_profiles;
            }
          }
          profile.SetMultiInfo(type_it->first, value_list);
        } else {
          *error_message= "All values must be lists";
          return autofill_profiles;
        }
      }
    }
    autofill_profiles.push_back(profile);
  }
  return autofill_profiles;
}
