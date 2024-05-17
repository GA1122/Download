void PanelSettingsMenuModel::ExtensionUninstallAccepted() {
  const Extension* extension = panel_->GetExtension();
  DCHECK(extension);

  panel_->browser()->GetProfile()->GetExtensionService()->
      UninstallExtension(extension->id(), false, NULL);
}
