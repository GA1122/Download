int32_t OutOfProcessInstance::PrintBegin(
    const PP_PrintSettings_Dev& print_settings) {
  int32_t ret = engine_->GetNumberOfPages();
  if (!ret)
    return 0;

  uint32_t supported_formats = engine_->QuerySupportedPrintOutputFormats();
  if ((print_settings.format & supported_formats) == 0)
    return 0;

  print_settings_.is_printing = true;
  print_settings_.pepper_print_settings = print_settings;
  engine_->PrintBegin();
  return ret;
}
