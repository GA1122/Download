const gfx::VectorIcon& AuthenticatorBleVerifyingSheetModel::GetStepIllustration(
    ImageColorScheme color_scheme) const {
  return color_scheme == ImageColorScheme::kDark ? kWebauthnBleDarkIcon
                                                 : kWebauthnBleIcon;
}
