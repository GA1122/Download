ChromePasswordManagerClient::GetAutofillDownloadManager() {
  autofill::ContentAutofillDriverFactory* factory =
      autofill::ContentAutofillDriverFactory::FromWebContents(web_contents());
  if (factory) {
    autofill::ContentAutofillDriver* driver =
        factory->DriverForFrame(web_contents()->GetMainFrame());
    if (driver) {
      autofill::AutofillManager* autofill_manager = driver->autofill_manager();
      if (autofill_manager)
        return autofill_manager->download_manager();
    }
  }
  return nullptr;
}
