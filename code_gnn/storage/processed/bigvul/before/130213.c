void LoadDpi(const wchar_t* printer,
             const wchar_t* port,
             const DEVMODE* devmode,
             PrinterSemanticCapsAndDefaults* caps) {
  std::vector<POINT> dpis;
  GetDeviceCapabilityArray(printer, port, DC_ENUMRESOLUTIONS, &dpis);

  for (size_t i = 0; i < dpis.size() ; ++i)
    caps->dpis.push_back(gfx::Size(dpis[i].x, dpis[i].y));

  if (devmode) {
    if ((devmode->dmFields & DM_PRINTQUALITY) && devmode->dmPrintQuality > 0) {
      caps->default_dpi.SetSize(devmode->dmPrintQuality,
                                devmode->dmPrintQuality);
      if (devmode->dmFields & DM_YRESOLUTION) {
        caps->default_dpi.set_height(devmode->dmYResolution);
      }
    }
  }
}
