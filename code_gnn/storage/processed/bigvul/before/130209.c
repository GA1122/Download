bool PrintBackendWin::GetPrinterCapsAndDefaults(
    const std::string& printer_name,
    PrinterCapsAndDefaults* printer_info) {
  ScopedXPSInitializer xps_initializer;
  if (!xps_initializer.initialized()) {
    return false;
  }
  if (!IsValidPrinter(printer_name)) {
    return false;
  }
  DCHECK(printer_info);
  HPTPROVIDER provider = NULL;
  std::wstring printer_name_wide = base::UTF8ToWide(printer_name);
  HRESULT hr = XPSModule::OpenProvider(printer_name_wide, 1, &provider);
  if (provider) {
    base::win::ScopedComPtr<IStream> print_capabilities_stream;
    hr = CreateStreamOnHGlobal(NULL, TRUE,
                               print_capabilities_stream.Receive());
    DCHECK(SUCCEEDED(hr));
    if (print_capabilities_stream) {
      base::win::ScopedBstr error;
      hr = XPSModule::GetPrintCapabilities(provider,
                                           NULL,
                                           print_capabilities_stream,
                                           error.Receive());
      DCHECK(SUCCEEDED(hr));
      if (FAILED(hr)) {
        return false;
      }
      hr = StreamOnHGlobalToString(print_capabilities_stream.get(),
                                   &printer_info->printer_capabilities);
      DCHECK(SUCCEEDED(hr));
      printer_info->caps_mime_type = "text/xml";
    }
    ScopedPrinterHandle printer_handle;
    if (printer_handle.OpenPrinter(printer_name_wide.c_str())) {
      scoped_ptr<DEVMODE, base::FreeDeleter> devmode_out(
          CreateDevMode(printer_handle, NULL));
      if (!devmode_out)
        return false;
      base::win::ScopedComPtr<IStream> printer_defaults_stream;
      hr = CreateStreamOnHGlobal(NULL, TRUE,
                                 printer_defaults_stream.Receive());
      DCHECK(SUCCEEDED(hr));
      if (printer_defaults_stream) {
        DWORD dm_size = devmode_out->dmSize + devmode_out->dmDriverExtra;
        hr = XPSModule::ConvertDevModeToPrintTicket(provider, dm_size,
            devmode_out.get(), kPTJobScope, printer_defaults_stream);
        DCHECK(SUCCEEDED(hr));
        if (SUCCEEDED(hr)) {
          hr = StreamOnHGlobalToString(printer_defaults_stream.get(),
                                       &printer_info->printer_defaults);
          DCHECK(SUCCEEDED(hr));
          printer_info->defaults_mime_type = "text/xml";
        }
      }
    }
    XPSModule::CloseProvider(provider);
  }
  return true;
}
