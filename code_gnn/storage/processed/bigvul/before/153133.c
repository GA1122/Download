PP_BrowserFont_Trusted_Weight WeightToBrowserFontTrustedWeight(int weight) {
  static_assert(PP_BROWSERFONT_TRUSTED_WEIGHT_100 == 0,
                "PP_BrowserFont_Trusted_Weight min");
  static_assert(PP_BROWSERFONT_TRUSTED_WEIGHT_900 == 8,
                "PP_BrowserFont_Trusted_Weight max");
  const int kMinimumWeight = 100;
  const int kMaximumWeight = 900;
  int normalized_weight =
      std::min(std::max(weight, kMinimumWeight), kMaximumWeight);
  normalized_weight = (normalized_weight / 100) - 1;
  return static_cast<PP_BrowserFont_Trusted_Weight>(normalized_weight);
}
