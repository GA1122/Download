void BrowserActionsContainer::Layout() {
  if (!resize_gripper_ || !chevron_)
    return;   
  if (browser_action_views_.size() == 0) {
    resize_gripper_->SetVisible(false);
    chevron_->SetVisible(false);
    return;
  }

  int x = 0;
  if (resize_gripper_->IsVisible()) {
    gfx::Size sz = resize_gripper_->GetPreferredSize();
    resize_gripper_->SetBounds(x, (height() - sz.height()) / 2 + 1,
                               sz.width() + kExtraResizeArea, sz.height());
    x += sz.width();
  }

  x += kHorizontalPadding;

  int last_x_of_icons = x +
      (browser_action_views_.size() * kButtonSize) +
      ((browser_action_views_.size() - 1) *
          kBrowserActionButtonPadding);

  int max_x = width() - kDividerHorizontalMargin - kChevronRightMargin;

  gfx::Size chevron_size;
  if (last_x_of_icons >= max_x && !suppress_chevron_) {
    chevron_->SetVisible(true);
    chevron_size = chevron_->GetPreferredSize();
    max_x -= chevron_size.width();
    chevron_->SetBounds(width() - chevron_size.width() - kChevronRightMargin,
                        kChevronTopMargin,
                        chevron_size.width(), chevron_size.height());
  } else {
    chevron_->SetVisible(false);
  }

  for (size_t i = 0; i < browser_action_views_.size(); ++i) {
    BrowserActionView* view = browser_action_views_[i];
    int padding = (i > 0) ? kBrowserActionButtonPadding : 0;
    if (x + kButtonSize + padding < max_x) {
      x += padding;
      view->SetBounds(x, 0, kButtonSize, height());
      view->SetVisible(true);
      x += kButtonSize;
    } else {
      view->SetVisible(false);
    }
  }
}
