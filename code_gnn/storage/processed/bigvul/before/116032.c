  virtual gfx::Size GetPreferredSize() {
    int width = kHorizOuterMargin;
    width += kIconSize;
    width += views::kPanelHorizMargin;
    width += kRightColumnWidth;
    width += 2 * views::kPanelHorizMargin;
    width += kHorizOuterMargin;

    int height = kVertOuterMargin;
    height += heading_->GetHeightForWidth(kRightColumnWidth);
    height += kVertInnerMargin;
    if (info_) {
      height += info_->GetHeightForWidth(kRightColumnWidth);
      height += kVertInnerMargin;
    }
    height += manage_->GetHeightForWidth(kRightColumnWidth);
    height += kVertOuterMargin;

    return gfx::Size(width, std::max(height, kIconSize + 2 * kVertOuterMargin));
  }
