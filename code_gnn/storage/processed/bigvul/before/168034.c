void AutofillManager::DisambiguateUploadTypes(FormStructure* form) {
  for (size_t i = 0; i < form->field_count(); ++i) {
    AutofillField* field = form->field(i);
    const ServerFieldTypeSet& upload_types = field->possible_types();

    if (upload_types.size() == 2) {
      if (upload_types.count(ADDRESS_HOME_LINE1) &&
          upload_types.count(ADDRESS_HOME_STREET_ADDRESS)) {
        AutofillManager::DisambiguateAddressUploadTypes(form, i);
      } else if (upload_types.count(PHONE_HOME_CITY_AND_NUMBER) &&
                 upload_types.count(PHONE_HOME_WHOLE_NUMBER)) {
        AutofillManager::DisambiguatePhoneUploadTypes(form, i);
      } else if ((upload_types.count(NAME_FULL) &&
                  upload_types.count(CREDIT_CARD_NAME_FULL)) ||
                 (upload_types.count(NAME_FIRST) &&
                  upload_types.count(CREDIT_CARD_NAME_FIRST)) ||
                 (upload_types.count(NAME_LAST) &&
                  upload_types.count(CREDIT_CARD_NAME_LAST))) {
        AutofillManager::DisambiguateNameUploadTypes(form, i, upload_types);
      }
    }
  }
}
