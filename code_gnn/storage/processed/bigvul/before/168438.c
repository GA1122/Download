Font PlatformFontSkia::DeriveFont(int size_delta,
                                  int style,
                                  Font::Weight weight) const {
  const int new_size = font_size_pixels_ + size_delta;
  DCHECK_GT(new_size, 0);

  std::string new_family = font_family_;
  bool success = true;
  sk_sp<SkTypeface> typeface =
      (weight == weight_ && style == style_)
          ? typeface_
          : CreateSkTypeface(style, weight, &new_family, &success);
  if (!success) {
    LOG(ERROR) << "Could not find any font: " << new_family << ", "
               << kFallbackFontFamilyName << ". Falling back to the default";
    return Font(new PlatformFontSkia);
  }

  FontRenderParamsQuery query;
  query.families.push_back(new_family);
  query.pixel_size = new_size;
  query.style = style;

  return Font(new PlatformFontSkia(std::move(typeface), new_family, new_size,
                                   style, weight,
                                   gfx::GetFontRenderParams(query, NULL)));
}
