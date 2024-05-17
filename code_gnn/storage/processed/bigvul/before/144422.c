gfx::Size ExpandableContainerView::DetailsView::GetPreferredSize() const {
  gfx::Size size = views::View::GetPreferredSize();
  return gfx::Size(size.width(), size.height() * state_);
}
