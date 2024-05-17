const char* GetFormFactorHintCommandLine(FormFactor form_factor) {
  switch (form_factor) {
    case FORM_FACTOR_DESKTOP:
      return switches::kFormFactorDesktop;
    case FORM_FACTOR_TABLET:
      return switches::kFormFactorTablet;
    case FORM_FACTOR_PHONE:
      return switches::kFormFactorPhone;
  }

  NOTREACHED();
  return nullptr;
}
