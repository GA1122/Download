void NavigationObserver::OnInstallPromptDone(
    ExtensionInstallPrompt::Result result) {
  if (in_progress_prompt_extension_id_.empty())
    return;

  ExtensionService* extension_service =
      extensions::ExtensionSystem::Get(profile_)->extension_service();
  const Extension* extension = extension_service->GetExtensionById(
      in_progress_prompt_extension_id_, true);
  CHECK(extension);

  if (result == ExtensionInstallPrompt::Result::ACCEPTED) {
    NavigationController* nav_controller =
        in_progress_prompt_navigation_controller_;
    CHECK(nav_controller);

    extension_service->GrantPermissionsAndEnableExtension(extension);
    nav_controller->Reload(content::ReloadType::NORMAL, true);
  } else {
    std::string histogram_name =
       result == ExtensionInstallPrompt::Result::USER_CANCELED
            ? "ReEnableCancel"
            : "ReEnableAbort";
    ExtensionService::RecordPermissionMessagesHistogram(extension,
                                                        histogram_name.c_str());
  }

  in_progress_prompt_extension_id_ = std::string();
  in_progress_prompt_navigation_controller_ = nullptr;
  extension_install_prompt_.reset();
}
