void PanelSettingsMenuModel::ExecuteCommand(int command_id) {
  const Extension* extension = panel_->GetExtension();
  DCHECK(extension);

  Browser* browser = panel_->browser();
  switch (command_id) {
    case COMMAND_NAME: {
      OpenURLParams params(
          extension->GetHomepageURL(), Referrer(), NEW_FOREGROUND_TAB,
          content::PAGE_TRANSITION_LINK, false);
      browser->OpenURL(params);
      break;
    }
    case COMMAND_CONFIGURE:
      DCHECK(!extension->options_url().is_empty());
      browser->GetProfile()->GetExtensionProcessManager()->OpenOptionsPage(
          extension, browser);
      break;
    case COMMAND_DISABLE:
      browser->GetProfile()->GetExtensionService()->DisableExtension(
          extension->id());
      break;
    case COMMAND_UNINSTALL:
      extension_uninstall_dialog_.reset(
          ExtensionUninstallDialog::Create(browser->GetProfile(), this));
      extension_uninstall_dialog_->ConfirmUninstall(extension);
      break;
    case COMMAND_MANAGE:
      browser->ShowOptionsTab(chrome::kExtensionsSubPage);
      break;
    default:
      NOTREACHED();
      break;
  }
}
