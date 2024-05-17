bool ExtensionContextMenuModel::IsCommandIdEnabled(int command_id) const {
  const Extension* extension = this->GetExtension();
  if (!extension)
    return false;

  if (command_id == CONFIGURE) {
    return extension->options_url().spec().length() > 0;
  } else if (command_id == NAME) {
    return extension->GetHomepageURL().is_valid();
  } else if (command_id == INSPECT_POPUP) {
    WebContents* contents = browser_->GetSelectedWebContents();
    if (!contents)
      return false;

    return extension_action_->HasPopup(ExtensionTabUtil::GetTabId(contents));
  } else if (command_id == DISABLE || command_id == UNINSTALL) {
    return Extension::UserMayDisable(extension->location());
  }
  return true;
}
