void AppLauncherHandler::ExtensionUninstallAccepted() {
  DCHECK(!extension_id_prompting_.empty());

  const Extension* extension =
      extension_service_->GetInstalledExtension(extension_id_prompting_);
  if (!extension)
    return;

  extension_service_->UninstallExtension(extension_id_prompting_,
                                         false  , NULL);
  CleanupAfterUninstall();
}
