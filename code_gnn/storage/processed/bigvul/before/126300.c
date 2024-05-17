void BrowserCommandController::UpdatePrintingState() {
  bool print_enabled = CanPrint(browser_);
  command_updater_.UpdateCommandEnabled(IDC_PRINT, print_enabled);
  command_updater_.UpdateCommandEnabled(IDC_ADVANCED_PRINT,
                                        CanAdvancedPrint(browser_));
  command_updater_.UpdateCommandEnabled(IDC_PRINT_TO_DESTINATION,
                                        print_enabled);
#if defined(OS_WIN)
  HMODULE metro_module = base::win::GetMetroModule();
  if (metro_module != NULL) {
    typedef void (*MetroEnablePrinting)(BOOL);
    MetroEnablePrinting metro_enable_printing =
        reinterpret_cast<MetroEnablePrinting>(
            ::GetProcAddress(metro_module, "MetroEnablePrinting"));
    if (metro_enable_printing)
      metro_enable_printing(print_enabled);
  }
#endif
}
