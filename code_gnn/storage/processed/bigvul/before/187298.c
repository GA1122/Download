  NotificationsNativeHandler::NotificationsNativeHandler(ScriptContext* context)
      : ObjectBackedNativeHandler(context) {
    RouteFunction(
      "GetNotificationImageSizes",
//       "GetNotificationImageSizes", "notifications",
        base::Bind(&NotificationsNativeHandler::GetNotificationImageSizes,
                   base::Unretained(this)));
  }