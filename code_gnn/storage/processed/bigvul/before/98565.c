void DetectTabLanguageFunction::GotLanguage(const std::string& language) {
  result_.reset(Value::CreateStringValue(language.c_str()));
  SendResponse(true);

  Release();   
}
