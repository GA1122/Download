void PrintPreviewHandler::SendInitialSettings(
    const std::string& default_printer,
    const std::string& cloud_print_data) {
  PrintPreviewUI* print_preview_ui = static_cast<PrintPreviewUI*>(
      web_ui()->GetController());

  base::DictionaryValue initial_settings;
  initial_settings.SetString(kInitiatorTabTitle,
                             print_preview_ui->initiator_tab_title());
  initial_settings.SetBoolean(printing::kSettingPreviewModifiable,
                              print_preview_ui->source_is_modifiable());
  initial_settings.SetString(printing::kSettingPrinterName,
                             default_printer);
  initial_settings.SetString(kCloudPrintData, cloud_print_data);
  initial_settings.SetBoolean(printing::kSettingHeaderFooterEnabled,
                              GetStickySettings()->headers_footers());
  initial_settings.SetInteger(printing::kSettingDuplexMode,
                              GetStickySettings()->duplex_mode());


#if defined(OS_MACOSX)
  bool kiosk_mode = false;   
#else
  CommandLine* cmdline = CommandLine::ForCurrentProcess();
  bool kiosk_mode = (cmdline->HasSwitch(switches::kKioskMode) &&
                     cmdline->HasSwitch(switches::kKioskModePrinting));
#endif
  initial_settings.SetBoolean(kPrintAutomaticallyInKioskMode, kiosk_mode);

  if (print_preview_ui->source_is_modifiable()) {
    GetStickySettings()->GetLastUsedMarginSettings(&initial_settings);
    GetNumberFormatAndMeasurementSystem(&initial_settings);
  }
  web_ui()->CallJavascriptFunction("setInitialSettings", initial_settings);
}
