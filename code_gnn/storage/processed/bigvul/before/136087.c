gfx::Size WebsiteSettingsPopupView::GetPreferredSize() const {
  if (header_ == nullptr && tabbed_pane_ == nullptr)
    return views::View::GetPreferredSize();

  int height = 0;
  if (header_)
    height += header_->GetPreferredSize().height();
  if (tabbed_pane_)
    height += tabbed_pane_->GetPreferredSize().height();

  int width = kPermissionsSectionContentMinWidth;
  if (site_data_content_)
    width = std::max(width, site_data_content_->GetPreferredSize().width());
  if (permissions_content_)
    width = std::max(width, permissions_content_->GetPreferredSize().width());
  width += kPermissionsSectionPaddingLeft;
  width = std::min(width, kMaxPopupWidth);

  return gfx::Size(width, height);
}
