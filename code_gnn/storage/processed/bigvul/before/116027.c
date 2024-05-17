bool PanelSettingsMenuModel::IsCommandIdEnabled(int command_id) const {
  const Extension* extension = panel_->GetExtension();
  DCHECK(extension);

  switch (command_id) {
    case COMMAND_NAME:
      return extension->GetHomepageURL().is_valid();
    case COMMAND_CONFIGURE:
      return extension->options_url().spec().length() > 0;
    case COMMAND_DISABLE:
    case COMMAND_UNINSTALL:
      return Extension::UserMayDisable(extension->location());
    case COMMAND_MANAGE:
      return true;
    default:
      NOTREACHED();
      return false;
  }
}
