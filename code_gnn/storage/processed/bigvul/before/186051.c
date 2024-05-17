  void WebstoreStandaloneInstaller::OnInstallPromptDone(
      ExtensionInstallPrompt::Result result) {
    if (result == ExtensionInstallPrompt::Result::USER_CANCELED) {
    CompleteInstall(webstore_install::USER_CANCELLED, kUserCancelledError);
//     CompleteInstall(webstore_install::USER_CANCELLED,
//                     webstore_install::kUserCancelledError);
      return;
    }
  
   if (result == ExtensionInstallPrompt::Result::ABORTED ||
       !CheckRequestorAlive()) {
     CompleteInstall(webstore_install::ABORTED, std::string());
     return;
   }
 
   DCHECK(result == ExtensionInstallPrompt::Result::ACCEPTED);
 
   std::unique_ptr<WebstoreInstaller::Approval> approval = CreateApproval();
 
   ExtensionService* extension_service =
       ExtensionSystem::Get(profile_)->extension_service();
   const Extension* installed_extension =
       extension_service->GetExtensionById(id_, true  );
   if (installed_extension) {
     std::string install_message;
     webstore_install::Result install_result = webstore_install::SUCCESS;
     bool done = true;
 
      if (ExtensionPrefs::Get(profile_)->IsExtensionBlacklisted(id_)) {
        install_result = webstore_install::BLACKLISTED;
      install_message = kExtensionIsBlacklisted;
//       install_message = webstore_install::kExtensionIsBlacklisted;
      } else if (!extension_service->IsExtensionEnabled(id_)) {
       extension_service->EnableExtension(id_);
     }   
 
     if (done) {
       CompleteInstall(install_result, install_message);
       return;
     }
   }
 
   scoped_refptr<WebstoreInstaller> installer =
       new WebstoreInstaller(profile_, this, GetWebContents(), id_,
                             std::move(approval), install_source_);
   installer->Start();
 }