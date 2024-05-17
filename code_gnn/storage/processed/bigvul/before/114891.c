    TestingAutomationProvider::GetAutofillFieldToStringMap() {
  std::map<AutofillFieldType, std::string> autofill_type_to_string;
  autofill_type_to_string[NAME_FIRST] = "NAME_FIRST";
  autofill_type_to_string[NAME_MIDDLE] = "NAME_MIDDLE";
  autofill_type_to_string[NAME_LAST] = "NAME_LAST";
  autofill_type_to_string[COMPANY_NAME] = "COMPANY_NAME";
  autofill_type_to_string[EMAIL_ADDRESS] = "EMAIL_ADDRESS";
  autofill_type_to_string[ADDRESS_HOME_LINE1] = "ADDRESS_HOME_LINE1";
  autofill_type_to_string[ADDRESS_HOME_LINE2] = "ADDRESS_HOME_LINE2";
  autofill_type_to_string[ADDRESS_HOME_CITY] = "ADDRESS_HOME_CITY";
  autofill_type_to_string[ADDRESS_HOME_STATE] = "ADDRESS_HOME_STATE";
  autofill_type_to_string[ADDRESS_HOME_ZIP] = "ADDRESS_HOME_ZIP";
  autofill_type_to_string[ADDRESS_HOME_COUNTRY] = "ADDRESS_HOME_COUNTRY";
  autofill_type_to_string[PHONE_HOME_COUNTRY_CODE] =
      "PHONE_HOME_COUNTRY_CODE";
  autofill_type_to_string[PHONE_HOME_CITY_CODE] = "PHONE_HOME_CITY_CODE";
  autofill_type_to_string[PHONE_HOME_WHOLE_NUMBER] =
      "PHONE_HOME_WHOLE_NUMBER";
  return autofill_type_to_string;
}
