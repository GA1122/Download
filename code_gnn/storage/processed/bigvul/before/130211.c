bool PrintBackendWin::GetPrinterSemanticCapsAndDefaults(
    const std::string& printer_name,
    PrinterSemanticCapsAndDefaults* printer_info) {
  ScopedPrinterHandle printer_handle;
  if (!printer_handle.OpenPrinter(base::UTF8ToWide(printer_name).c_str())) {
    LOG(WARNING) << "Failed to open printer, error = " << GetLastError();
    return false;
  }

  PrinterInfo5 info_5;
  if (!info_5.Init(printer_handle))
    return false;
  const wchar_t* name = info_5.get()->pPrinterName;
  const wchar_t* port = info_5.get()->pPortName;
  DCHECK_EQ(name, base::UTF8ToUTF16(printer_name));

  PrinterSemanticCapsAndDefaults caps;

  scoped_ptr<DEVMODE, base::FreeDeleter> user_settings =
      CreateDevMode(printer_handle, NULL);
  if (user_settings) {
    if (user_settings->dmFields & DM_COLOR)
      caps.color_default = (user_settings->dmColor == DMCOLOR_COLOR);

    if (user_settings->dmFields & DM_DUPLEX) {
      switch (user_settings->dmDuplex) {
      case DMDUP_SIMPLEX:
        caps.duplex_default = SIMPLEX;
        break;
      case DMDUP_VERTICAL:
        caps.duplex_default = LONG_EDGE;
        break;
      case DMDUP_HORIZONTAL:
        caps.duplex_default = SHORT_EDGE;
        break;
      default:
        NOTREACHED();
      }
    }

    if (user_settings->dmFields & DM_COLLATE)
      caps.collate_default = (user_settings->dmCollate == DMCOLLATE_TRUE);
  } else {
    LOG(WARNING) << "Fallback to color/simplex mode.";
    caps.color_default = caps.color_changeable;
    caps.duplex_default = SIMPLEX;
  }

  caps.color_changeable =
      (DeviceCapabilities(name, port, DC_COLORDEVICE, NULL, NULL) == 1);
  caps.color_model = printing::COLOR;
  caps.bw_model = printing::GRAY;

  caps.duplex_capable =
      (DeviceCapabilities(name, port, DC_DUPLEX, NULL, NULL) == 1);

  caps.collate_capable =
      (DeviceCapabilities(name, port, DC_COLLATE, NULL, NULL) == 1);

  caps.copies_capable =
      (DeviceCapabilities(name, port, DC_COPIES, NULL, NULL) > 1);

  LoadPaper(name, port, user_settings.get(), &caps);
  LoadDpi(name, port, user_settings.get(), &caps);

  *printer_info = caps;
  return true;
}
