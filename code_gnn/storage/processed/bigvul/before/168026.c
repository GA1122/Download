AutofillManager::AutofillManager(
    AutofillDriver* driver,
    AutofillClient* client,
    PersonalDataManager* personal_data,
    const std::string app_locale,
    AutofillDownloadManagerState enable_download_manager)
    : AutofillHandler(driver),
      client_(client),
      payments_client_(std::make_unique<payments::PaymentsClient>(
          driver->GetURLLoaderFactory(),
          client->GetPrefs(),
          client->GetIdentityManager(),
           this,
           nullptr,
          driver->IsIncognito())),
      app_locale_(app_locale),
      personal_data_(personal_data),
      form_data_importer_(
          std::make_unique<FormDataImporter>(client,
                                             payments_client_.get(),
                                             personal_data,
                                             app_locale_)),
      field_filler_(app_locale, client->GetAddressNormalizer()),
      autocomplete_history_manager_(
          std::make_unique<AutocompleteHistoryManager>(driver, client)),
      form_interactions_ukm_logger_(
          std::make_unique<AutofillMetrics::FormInteractionsUkmLogger>(
              client->GetUkmRecorder())),
      address_form_event_logger_(
          std::make_unique<AutofillMetrics::FormEventLogger>(
               false,
              form_interactions_ukm_logger_.get())),
      credit_card_form_event_logger_(
          std::make_unique<AutofillMetrics::FormEventLogger>(
               true,
              form_interactions_ukm_logger_.get())),
#if defined(OS_ANDROID) || defined(OS_IOS)
      autofill_assistant_(this),
#endif
      weak_ptr_factory_(this) {
  DCHECK(driver);
  DCHECK(client_);
  if (enable_download_manager == ENABLE_AUTOFILL_DOWNLOAD_MANAGER) {
    download_manager_.reset(new AutofillDownloadManager(driver, this));
  }
  CountryNames::SetLocaleString(app_locale_);
  if (personal_data_ && client_)
    personal_data_->OnSyncServiceInitialized(client_->GetSyncService());
}
