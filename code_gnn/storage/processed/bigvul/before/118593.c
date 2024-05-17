void AppLauncherHandler::HandleReorderApps(const base::ListValue* args) {
  CHECK(args->GetSize() == 2);

  std::string dragged_app_id;
  const base::ListValue* app_order;
  CHECK(args->GetString(0, &dragged_app_id));
  CHECK(args->GetList(1, &app_order));

  std::string predecessor_to_moved_ext;
  std::string successor_to_moved_ext;
  for (size_t i = 0; i < app_order->GetSize(); ++i) {
    std::string value;
    if (app_order->GetString(i, &value) && value == dragged_app_id) {
      if (i > 0)
        CHECK(app_order->GetString(i - 1, &predecessor_to_moved_ext));
      if (i + 1 < app_order->GetSize())
        CHECK(app_order->GetString(i + 1, &successor_to_moved_ext));
      break;
    }
  }

  base::AutoReset<bool> auto_reset(&ignore_changes_, true);
  ExtensionPrefs* extension_prefs =
      ExtensionPrefs::Get(extension_service_->GetBrowserContext());
  extension_prefs->SetAppDraggedByUser(dragged_app_id);
  extension_prefs->app_sorting()->OnExtensionMoved(
      dragged_app_id, predecessor_to_moved_ext, successor_to_moved_ext);
}
