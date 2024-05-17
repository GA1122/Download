ListValue* TestingAutomationProvider::GetListFromAutofillProfiles(
    const std::vector<AutofillProfile*>& autofill_profiles) {
  ListValue* profiles = new ListValue;

  std::map<AutofillFieldType, std::string> autofill_type_to_string
      = GetAutofillFieldToStringMap();

  for (std::vector<AutofillProfile*>::const_iterator it =
           autofill_profiles.begin();
       it != autofill_profiles.end(); ++it) {
    AutofillProfile* profile = *it;
    DictionaryValue* profile_info = new DictionaryValue;
    for (std::map<AutofillFieldType, std::string>::iterator
         type_it = autofill_type_to_string.begin();
         type_it != autofill_type_to_string.end(); ++type_it) {
      std::vector<string16> value_list;
      profile->GetMultiInfo(type_it->first, &value_list);
      ListValue* values_to_return = new ListValue;
      for (std::vector<string16>::iterator value_it = value_list.begin();
           value_it != value_list.end(); ++value_it) {
        string16 value = *value_it;
        if (value.length())   
          values_to_return->Append(new StringValue(value));
      }
      if (!values_to_return->empty())
        profile_info->Set(type_it->second, values_to_return);
    }
    profiles->Append(profile_info);
  }
  return profiles;
}
