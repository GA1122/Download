variations::Study_FormFactor GetCurrentFormFactor() {
  switch (ui::GetDeviceFormFactor()) {
    case ui::DEVICE_FORM_FACTOR_PHONE:
      return variations::Study_FormFactor_PHONE;
    case ui::DEVICE_FORM_FACTOR_TABLET:
      return variations::Study_FormFactor_TABLET;
    case ui::DEVICE_FORM_FACTOR_DESKTOP:
      return variations::Study_FormFactor_DESKTOP;
  }
  NOTREACHED();
  return variations::Study_FormFactor_DESKTOP;
}
