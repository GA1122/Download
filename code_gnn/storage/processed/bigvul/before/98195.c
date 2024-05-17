TranslateManager::TranslateManager()
    : ALLOW_THIS_IN_INITIALIZER_LIST(method_factory_(this)),
      translate_script_expiration_delay_(kTranslateScriptExpirationDelayMS),
      translate_script_request_pending_(false) {
  notification_registrar_.Add(this, NotificationType::NAV_ENTRY_COMMITTED,
                              NotificationService::AllSources());
  notification_registrar_.Add(this, NotificationType::TAB_LANGUAGE_DETERMINED,
                              NotificationService::AllSources());
  notification_registrar_.Add(this, NotificationType::PAGE_TRANSLATED,
                              NotificationService::AllSources());
}
