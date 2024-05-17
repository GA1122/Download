std::string ChromePasswordManagerClient::GetPageLanguage() const {
  auto* translate_manager =
      ChromeTranslateClient::GetManagerFromWebContents(web_contents());
  if (translate_manager)
    return translate_manager->GetLanguageState().original_language();
  return std::string();
}
