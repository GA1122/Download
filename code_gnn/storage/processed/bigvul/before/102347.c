DictionaryValue* PolicyStatusInfo::GetDictionaryValue() const {
  string16 level_string = GetPolicyLevelString(level);
  string16 source_type_string = GetSourceTypeString(source_type);
  string16 status_message =
      status == ENFORCED ? l10n_util::GetStringUTF16(IDS_OK) : error_message;
  DictionaryValue* result = new DictionaryValue();
  result->SetString(std::string(kNameDictPath), name);
  result->SetString(std::string(kLevelDictPath), level_string);
  result->SetString(std::string(kSourceTypeDictPath), source_type_string);
  result->Set(std::string(kValueDictPath), value->DeepCopy());
  result->SetBoolean(std::string(kSetDictPath), level != LEVEL_UNDEFINED);
  result->SetString(std::string(kStatusDictPath), status_message);

  return result;
}
