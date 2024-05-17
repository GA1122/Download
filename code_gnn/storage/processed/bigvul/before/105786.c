bool Label::GetTooltipText(const gfx::Point& p, std::wstring* tooltip) {
  DCHECK(tooltip);

  if (!tooltip_text_.empty()) {
    tooltip->assign(UTF16ToWideHack(tooltip_text_));
    return true;
  }

  if (!is_multi_line_ &&
      (font_.GetStringWidth(text_) > GetAvailableRect().width())) {
    *tooltip = UTF16ToWideHack(text_);
    return true;
  }
  return false;
}
