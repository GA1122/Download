scoped_ptr<base::DictionaryValue> DetectedLanguage::ToDictionary() const {
  scoped_ptr<base::DictionaryValue> dict_value(new base::DictionaryValue());
  dict_value->SetString("language", language.c_str());
  dict_value->SetInteger("percentage", percentage);
  return dict_value;
}
