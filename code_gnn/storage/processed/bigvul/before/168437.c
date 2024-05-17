sk_sp<SkTypeface> CreateSkTypeface(bool italic,
                                   gfx::Font::Weight weight,
                                   std::string* family,
                                   bool* out_success) {
  DCHECK(family);
  TRACE_EVENT0("fonts", "gfx::CreateSkTypeface");

  const int font_weight = (weight == Font::Weight::INVALID)
                              ? static_cast<int>(Font::Weight::NORMAL)
                              : static_cast<int>(weight);
  SkFontStyle sk_style(
      font_weight, SkFontStyle::kNormal_Width,
      italic ? SkFontStyle::kItalic_Slant : SkFontStyle::kUpright_Slant);
  sk_sp<SkTypeface> typeface;
  {
    TRACE_EVENT1(TRACE_DISABLED_BY_DEFAULT("fonts"), "SkTypeface::MakeFromName",
                 "family", *family);
    typeface = SkTypeface::MakeFromName(family->c_str(), sk_style);
  }
  if (!typeface) {
    TRACE_EVENT1(TRACE_DISABLED_BY_DEFAULT("fonts"), "SkTypeface::MakeFromName",
                 "family", kFallbackFontFamilyName);
    typeface = sk_sp<SkTypeface>(
        SkTypeface::MakeFromName(kFallbackFontFamilyName, sk_style));
    if (!typeface) {
      *out_success = false;
      return nullptr;
    }
    *family = kFallbackFontFamilyName;
  }
  *out_success = true;
  return typeface;
}
