AuthenticatorAlreadyRegisteredErrorModel::GetStepIllustration(
    ImageColorScheme color_scheme) const {
  return color_scheme == ImageColorScheme::kDark ? kWebauthnErrorDarkIcon
                                                 : kWebauthnErrorIcon;
}
