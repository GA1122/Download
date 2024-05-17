bool AutofillPopupSuggestionView::ShouldUseCustomFontWeightForPrimaryInfo(
    gfx::Font::Weight* font_weight) const {
  switch (autofill::GetForcedFontWeight()) {
    case ForcedFontWeight::kDefault:
      return false;

    case ForcedFontWeight::kMedium:
      *font_weight = views::TypographyProvider::MediumWeightForUI();
      return true;

    case ForcedFontWeight::kBold:
      *font_weight = gfx::Font::Weight::BOLD;
      return true;
  }
}
