bool ImageView::GetTooltipText(const gfx::Point& p, std::wstring* tooltip) {
  if (tooltip_text_.empty())
    return false;

  *tooltip = GetTooltipText();
  return true;
}
