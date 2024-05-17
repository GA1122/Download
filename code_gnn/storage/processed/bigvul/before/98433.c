void ImageView::SetTooltipText(const std::wstring& tooltip) {
  tooltip_text_ = tooltip;
  SetAccessibleName(tooltip);
}
