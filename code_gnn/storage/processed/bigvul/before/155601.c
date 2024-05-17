const gfx::VectorIcon& AuthenticatorPaaskSheetModel::GetStepIllustration(
    ImageColorScheme color_scheme) const {
  return color_scheme == ImageColorScheme::kDark ? kWebauthnPhoneDarkIcon
                                                 : kWebauthnPhoneIcon;
}
