void ExpandableContainerView::UpdateArrowToggle(bool expanded) {
  gfx::ImageSkia icon = gfx::CreateVectorIcon(
      expanded ? gfx::VectorIconId::FIND_PREV : gfx::VectorIconId::FIND_NEXT,
      16, gfx::kChromeIconGrey);
  arrow_toggle_->SetImage(views::Button::STATE_NORMAL, &icon);
}
