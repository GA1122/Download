void PrintPreviewHandler::HandleGetInitialSettings(const ListValue*  ) {
  printing::StickySettings* sticky_settings = GetStickySettings();
  sticky_settings->RestoreFromPrefs(Profile::FromBrowserContext(
      preview_web_contents()->GetBrowserContext())->GetPrefs());
  if (sticky_settings->printer_name()) {
    std::string cloud_print_data;
    if (sticky_settings->printer_cloud_print_data())
      cloud_print_data = *sticky_settings->printer_cloud_print_data();
    SendInitialSettings(*sticky_settings->printer_name(), cloud_print_data);
  } else {
    scoped_refptr<PrintSystemTaskProxy> task =
        new PrintSystemTaskProxy(AsWeakPtr(),
                                 print_backend_.get(),
                                 has_logged_printers_count_);
    BrowserThread::PostTask(
        BrowserThread::FILE, FROM_HERE,
        base::Bind(&PrintSystemTaskProxy::GetDefaultPrinter, task.get()));
  }
  SendCloudPrintEnabled();
}
