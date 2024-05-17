void Label::CalculateDrawStringParams(std::wstring* paint_text,
                                      gfx::Rect* text_bounds,
                                      int* flags) const {
  DCHECK(paint_text && text_bounds && flags);

  if (url_set_) {
    *paint_text = UTF16ToWideHack(
        ui::ElideUrl(url_, font_, GetAvailableRect().width(), std::string()));

    *paint_text = UTF16ToWide(base::i18n::GetDisplayStringInLTRDirectionality(
        WideToUTF16(*paint_text)));
  } else if (elide_in_middle_) {
    *paint_text = UTF16ToWideHack(ui::ElideText(text_,
        font_, GetAvailableRect().width(), true));
  } else {
    *paint_text = UTF16ToWideHack(text_);
  }

  *text_bounds = GetTextBounds();
  *flags = ComputeMultiLineFlags();
}
