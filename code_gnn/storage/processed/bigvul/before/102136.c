JsArgList SyncManager::SyncInternal::GetNotificationInfo(
    const JsArgList& args) {
  ListValue return_args;
  return_args.Append(NotificationInfoToValue(notification_info_map_));
  return JsArgList(&return_args);
}
