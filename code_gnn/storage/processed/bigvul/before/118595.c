void AppLauncherHandler::HandleSetLaunchType(const base::ListValue* args) {
  std::string extension_id;
  double launch_type;
  CHECK(args->GetString(0, &extension_id));
  CHECK(args->GetDouble(1, &launch_type));

  const Extension* extension =
      extension_service_->GetExtensionById(extension_id, true);
  if (!extension)
    return;

  base::AutoReset<bool> auto_reset(&ignore_changes_, true);

  extensions::SetLaunchType(
      extension_service_,
      extension_id,
      static_cast<extensions::LaunchType>(static_cast<int>(launch_type)));
}
