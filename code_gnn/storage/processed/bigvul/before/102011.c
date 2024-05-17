PrintingContext::Result PrintingContextCairo::UseDefaultSettings() {
  DCHECK(!in_print_job_);

  ResetSettings();
#if defined(OS_CHROMEOS)
  int dpi = 300;
  gfx::Size physical_size_device_units;
  gfx::Rect printable_area_device_units;
  int32_t width = 0;
  int32_t height = 0;
  UErrorCode error = U_ZERO_ERROR;
  ulocdata_getPaperSize(app_locale_.c_str(), &height, &width, &error);
  if (error != U_ZERO_ERROR) {
    LOG(WARNING) << "ulocdata_getPaperSize failed, using 8.5 x 11, error: "
                 << error;
    width = static_cast<int>(8.5 * dpi);
    height = static_cast<int>(11 * dpi);
  } else {
    width = static_cast<int>(ConvertUnitDouble(width, 25.4, 1.0) * dpi);
    height = static_cast<int>(ConvertUnitDouble(height, 25.4, 1.0) * dpi);
  }

  physical_size_device_units.SetSize(width, height);
  printable_area_device_units.SetRect(
      static_cast<int>(PrintSettingsInitializerGtk::kLeftMarginInInch * dpi),
      static_cast<int>(PrintSettingsInitializerGtk::kTopMarginInInch * dpi),
      width - (PrintSettingsInitializerGtk::kLeftMarginInInch +
          PrintSettingsInitializerGtk::kRightMarginInInch) * dpi,
      height - (PrintSettingsInitializerGtk::kTopMarginInInch +
          PrintSettingsInitializerGtk::kBottomMarginInInch) * dpi);

  settings_.set_dpi(dpi);
  settings_.SetPrinterPrintableArea(physical_size_device_units,
                                    printable_area_device_units,
                                    dpi);
#else
  if (!print_dialog_) {
    print_dialog_ = create_dialog_func_(this);
    print_dialog_->AddRefToDialog();
  }
  print_dialog_->UseDefaultSettings();
#endif   

  return OK;
}
