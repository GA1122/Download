AutofillManager::AutofillManager(
    AutofillDriver* driver,
    AutofillClient* client,
    const std::string& app_locale,
    AutofillDownloadManagerState enable_download_manager)
    : AutofillManager(driver,
                      client,
                      client->GetPersonalDataManager(),
                      app_locale,
                      enable_download_manager) {}
