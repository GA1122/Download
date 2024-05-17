void MakeSectionBold(views::StyledLabel* label,
                     const base::string16& text,
                     const base::Optional<int>& bold_start,
                     int bold_length) {
  auto create_style = [&](bool is_bold) {
    views::StyledLabel::RangeStyleInfo style;
    if (is_bold) {
      style.custom_font = label->GetDefaultFontList().Derive(
          0, gfx::Font::FontStyle::NORMAL, gfx::Font::Weight::BOLD);
    }
    style.override_color = SK_ColorWHITE;
    return style;
  };

  auto add_style = [&](const views::StyledLabel::RangeStyleInfo& style,
                       int start, int end) {
    if (start >= end)
      return;

    label->AddStyleRange(gfx::Range(start, end), style);
  };

  views::StyledLabel::RangeStyleInfo regular_style =
      create_style(false  );
  views::StyledLabel::RangeStyleInfo bold_style =
      create_style(true  );
  if (!bold_start || bold_length == 0) {
    add_style(regular_style, 0, text.length());
    return;
  }

  add_style(regular_style, 0, *bold_start - 1);
  add_style(bold_style, *bold_start, *bold_start + bold_length);
  add_style(regular_style, *bold_start + bold_length + 1, text.length());
}
