void PrintDialogGtk::InitPrintSettings(const PageRanges& page_ranges) {
  PrintSettings settings;
  printing::PrintSettingsInitializerGtk::InitPrintSettings(
      gtk_settings_, page_setup_, page_ranges, false, &settings);
  context_->InitWithSettings(settings);
}
