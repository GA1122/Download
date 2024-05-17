void TranslateInfoBarDelegate::ReportLanguageDetectionError() {
  TranslateManager* manager =
      TranslateTabHelper::GetManagerFromWebContents(GetWebContents());
  if (!manager)
    return;
  manager->ReportLanguageDetectionError();
}
