AuthenticatorBleDeviceSelectionSheetModel::GetStepIllustration(
    ImageColorScheme color_scheme) const {
  return color_scheme == ImageColorScheme::kDark ? kWebauthnBleNameDarkIcon
                                                 : kWebauthnBleNameIcon;
}
