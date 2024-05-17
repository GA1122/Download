const Extension* ExtensionBrowserTest::InstallOrUpdateExtension(
    const std::string& id,
    const base::FilePath& path,
    InstallUIType ui_type,
    int expected_change,
    Manifest::Location install_source,
    Browser* browser,
    Extension::InitFromValueFlags creation_flags,
    bool install_immediately,
    bool grant_permissions) {
  ExtensionService* service =
      extensions::ExtensionSystem::Get(profile())->extension_service();
  ExtensionRegistry* registry = ExtensionRegistry::Get(profile());
  size_t num_before = registry->enabled_extensions().size();

  {
    std::unique_ptr<ScopedTestDialogAutoConfirm> prompt_auto_confirm;
    if (ui_type == INSTALL_UI_TYPE_CANCEL) {
      prompt_auto_confirm.reset(new ScopedTestDialogAutoConfirm(
          ScopedTestDialogAutoConfirm::CANCEL));
    } else if (ui_type == INSTALL_UI_TYPE_NORMAL) {
      prompt_auto_confirm.reset(new ScopedTestDialogAutoConfirm(
          ScopedTestDialogAutoConfirm::NONE));
    } else if (ui_type == INSTALL_UI_TYPE_AUTO_CONFIRM) {
      prompt_auto_confirm.reset(new ScopedTestDialogAutoConfirm(
          ScopedTestDialogAutoConfirm::ACCEPT));
    }

    base::FilePath crx_path = path;
    if (crx_path.Extension() != FILE_PATH_LITERAL(".crx")) {
      crx_path = PackExtension(path);
    }
    if (crx_path.empty())
      return NULL;

    std::unique_ptr<ExtensionInstallPrompt> install_ui;
    if (prompt_auto_confirm) {
      install_ui.reset(new ExtensionInstallPrompt(
         browser->tab_strip_model()->GetActiveWebContents()));
    }
    scoped_refptr<extensions::CrxInstaller> installer(
        extensions::CrxInstaller::Create(service, std::move(install_ui)));
    installer->set_expected_id(id);
    installer->set_creation_flags(creation_flags);
    installer->set_install_source(install_source);
    installer->set_install_immediately(install_immediately);
    installer->set_allow_silent_install(grant_permissions);
    if (!installer->is_gallery_install()) {
      installer->set_off_store_install_allow_reason(
          extensions::CrxInstaller::OffStoreInstallAllowedInTest);
    }

    observer_->Watch(
        extensions::NOTIFICATION_CRX_INSTALLER_DONE,
        content::Source<extensions::CrxInstaller>(installer.get()));

    installer->InstallCrx(crx_path);

    observer_->Wait();
  }

  size_t num_after = registry->enabled_extensions().size();
  EXPECT_EQ(num_before + expected_change, num_after);
  if (num_before + expected_change != num_after) {
    VLOG(1) << "Num extensions before: " << base::SizeTToString(num_before)
            << " num after: " << base::SizeTToString(num_after)
            << " Installed extensions follow:";

    for (const scoped_refptr<const Extension>& extension :
         registry->enabled_extensions())
      VLOG(1) << "  " << extension->id();

    VLOG(1) << "Errors follow:";
    const std::vector<base::string16>* errors =
        ExtensionErrorReporter::GetInstance()->GetErrors();
    for (std::vector<base::string16>::const_iterator iter = errors->begin();
         iter != errors->end(); ++iter)
      VLOG(1) << *iter;

    return NULL;
  }

  if (!observer_->WaitForExtensionViewsToLoad())
    return NULL;
  return service->GetExtensionById(last_loaded_extension_id(), false);
}
