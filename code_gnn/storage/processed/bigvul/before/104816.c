void ExtensionService::RecordPermissionMessagesHistogram(
    const Extension* e, const char* histogram) {
  base::Histogram* counter = base::LinearHistogram::FactoryGet(
      histogram,
      1,
      Extension::PermissionMessage::ID_ENUM_BOUNDARY,
      Extension::PermissionMessage::ID_ENUM_BOUNDARY + 1,
      base::Histogram::kUmaTargetedHistogramFlag);

  std::vector<Extension::PermissionMessage> permissions =
      e->GetPermissionMessages();
  if (permissions.empty()) {
    counter->Add(Extension::PermissionMessage::ID_NONE);
  } else {
    std::vector<Extension::PermissionMessage>::iterator it;
    for (it = permissions.begin(); it != permissions.end(); ++it)
      counter->Add(it->message_id());
  }
}
