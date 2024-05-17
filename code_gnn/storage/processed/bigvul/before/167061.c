void ExpandableContainerView::UpdateArrowToggle(bool expanded) {
  gfx::ImageSkia icon = gfx::CreateVectorIcon(
      expanded ? kCaretUpIcon : kCaretDownIcon, gfx::kChromeIconGrey);
  arrow_toggle_->SetImage(views::Button::STATE_NORMAL, &icon);
}
