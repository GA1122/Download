void TabsDetectLanguageFunction::GotLanguage(const std::string& language) {
  SetResult(std::make_unique<base::Value>(language));
  SendResponse(true);

  Release();   
}
