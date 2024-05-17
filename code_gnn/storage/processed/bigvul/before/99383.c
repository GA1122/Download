PrintingContext::Result PrintingContext::ParseDialogResult(
    const PRINTDLG& dialog_options) {
  ResetSettings();

  DEVMODE* dev_mode = NULL;
  if (dialog_options.hDevMode) {
    dev_mode =
        reinterpret_cast<DEVMODE*>(GlobalLock(dialog_options.hDevMode));
    DCHECK(dev_mode);
  }

  std::wstring device_name;
  if (dialog_options.hDevNames) {
    DEVNAMES* dev_names =
        reinterpret_cast<DEVNAMES*>(GlobalLock(dialog_options.hDevNames));
    DCHECK(dev_names);
    if (dev_names) {
      device_name =
          reinterpret_cast<const wchar_t*>(
              reinterpret_cast<const wchar_t*>(dev_names) +
                  dev_names->wDeviceOffset);
      GlobalUnlock(dialog_options.hDevNames);
    }
  }

  bool success = false;
  if (dev_mode && !device_name.empty()) {
    context_ = dialog_options.hDC;
    success = InitializeSettings(*dev_mode, device_name, NULL, 0, false);
  }

  if (!success && dialog_options.hDC) {
    DeleteDC(dialog_options.hDC);
    context_ = NULL;
  }

  if (dev_mode) {
    GlobalUnlock(dialog_options.hDevMode);
  }

  if (dialog_options.hDevMode != NULL)
    GlobalFree(dialog_options.hDevMode);
  if (dialog_options.hDevNames != NULL)
    GlobalFree(dialog_options.hDevNames);

  return context_ ? OK : FAILED;
}
