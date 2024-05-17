void ExtensionContextMenuModel::ExecuteCommand(int command_id) {
  const Extension* extension = GetExtension();
  if (!extension)
    return;

  switch (command_id) {
    case NAME: {
      OpenURLParams params(extension->GetHomepageURL(), Referrer(),
                           NEW_FOREGROUND_TAB, content::PAGE_TRANSITION_LINK,
                           false);
      browser_->OpenURL(params);
      break;
    }
    case CONFIGURE:
      DCHECK(!extension->options_url().is_empty());
      profile_->GetExtensionProcessManager()->OpenOptionsPage(extension,
                                                              browser_);
      break;
    case HIDE: {
      ExtensionService* extension_service = profile_->GetExtensionService();
      extension_service->SetBrowserActionVisibility(extension, false);
      break;
    }
    case DISABLE: {
      ExtensionService* extension_service = profile_->GetExtensionService();
      extension_service->DisableExtension(extension_id_);
      break;
    }
    case UNINSTALL: {
      AddRef();   
      extension_uninstall_dialog_.reset(
          ExtensionUninstallDialog::Create(profile_, this));
      extension_uninstall_dialog_->ConfirmUninstall(extension);
      break;
    }
    case MANAGE: {
      browser_->ShowOptionsTab(chrome::kExtensionsSubPage);
      break;
    }
    case INSPECT_POPUP: {
      delegate_->InspectPopup(extension_action_);
      break;
    }
    default:
     NOTREACHED() << "Unknown option";
     break;
  }
}
