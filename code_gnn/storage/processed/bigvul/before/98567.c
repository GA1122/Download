void DetectTabLanguageFunction::Observe(NotificationType type,
                                        const NotificationSource& source,
                                        const NotificationDetails& details) {
  std::string language;
  if (type == NotificationType::TAB_LANGUAGE_DETERMINED)
    language = *Details<std::string>(details).ptr();

  registrar_.RemoveAll();

  GotLanguage(language);
}
