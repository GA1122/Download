void I18NCustomBindings::DetectTextLanguage(
    const v8::FunctionCallbackInfo<v8::Value>& args) {
  CHECK(args.Length() == 1);
  CHECK(args[0]->IsString());

  std::string text = *v8::String::Utf8Value(args[0]);
  CLD2::CLDHints cldhints = {nullptr, "", CLD2::UNKNOWN_ENCODING,
                             CLD2::UNKNOWN_LANGUAGE};

  bool is_plain_text = true;   
  int flags = 0;               
  int text_bytes;              
  int valid_prefix_bytes;      
  double normalized_score[kCldNumLangs];

  CLD2::Language languages[kCldNumLangs];
  int percents[kCldNumLangs];
  bool is_reliable = false;

  int cld_language = CLD2::ExtDetectLanguageSummaryCheckUTF8(
      text.c_str(), static_cast<int>(text.size()), is_plain_text, &cldhints,
      flags, languages, percents, normalized_score,
      nullptr,   
      &text_bytes, &is_reliable, &valid_prefix_bytes);

  if (valid_prefix_bytes < static_cast<int>(text.size()) &&
      cld_language == CLD2::UNKNOWN_LANGUAGE) {
    CLD2::ExtDetectLanguageSummary(
        text.c_str(), valid_prefix_bytes, is_plain_text, &cldhints, flags,
        languages, percents, normalized_score,
        nullptr,   
        &text_bytes, &is_reliable);
  }

  LanguageDetectionResult result(is_reliable);
  InitDetectedLanguages(languages, percents, &result.languages);

  args.GetReturnValue().Set(result.ToValue(context()));
}
