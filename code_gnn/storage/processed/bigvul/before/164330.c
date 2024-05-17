void TabsDetectLanguageFunction::Observe(
    int type,
    const content::NotificationSource& source,
    const content::NotificationDetails& details) {
  std::string language;
  if (type == chrome::NOTIFICATION_TAB_LANGUAGE_DETERMINED) {
    const translate::LanguageDetectionDetails* lang_det_details =
        content::Details<const translate::LanguageDetectionDetails>(details)
            .ptr();
    language = lang_det_details->adopted_language;
  }

  registrar_.RemoveAll();

  GotLanguage(language);
}
