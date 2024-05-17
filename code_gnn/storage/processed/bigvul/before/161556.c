base::string16 GetUninstallSurveyUrl() {
  const wchar_t kSurveyUrl[] = L"https://support.google.com/chrome/"
                               L"contact/chromeuninstall3?hl=$1";
  return LocalizeUrl(kSurveyUrl);
}
